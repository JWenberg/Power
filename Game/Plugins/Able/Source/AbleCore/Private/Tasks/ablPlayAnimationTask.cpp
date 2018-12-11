// Copyright (c) 2016 - 2018 Extra Life Studios, LLC. All rights reserved.

#include "Tasks/ablPlayAnimationTask.h"

#include "ablAbility.h"
#include "ablAbilityComponent.h"
#include "AbleCorePrivate.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimNode_AbilityAnimPlayer.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "Animation/AnimStateMachineTypes.h"
#include "Animation/AnimNode_StateMachine.h"

#include "Components/SkeletalMeshComponent.h"

#define LOCTEXT_NAMESPACE "AblAbilityTask"

UAblPlayAnimationTaskScratchPad::UAblPlayAnimationTaskScratchPad()
{

}

UAblPlayAnimationTaskScratchPad::~UAblPlayAnimationTaskScratchPad()
{

}

UAblPlayAnimationTask::UAblPlayAnimationTask(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	m_AnimationAsset(nullptr),
	m_AnimationMode(EAblPlayAnimationTaskAnimMode::SingleNode),
	m_StateMachineName(),
	m_AbilityStateName(),
	m_Loop(false),
	m_SlotName(TEXT("Ability")),
	m_PlayRate(1.0f),
	m_ScaleWithAbilityPlayRate(true),
	m_StopAnimationOnInterrupt(true),
	m_ClearQueuedAnimationOnInterrupt(false),
	m_UseDynamicAnimationAsset(false),
	m_EventName(NAME_None),
	m_PlayOnServer(false)
{

}

UAblPlayAnimationTask::~UAblPlayAnimationTask()
{

}

void UAblPlayAnimationTask::OnTaskStart(const TWeakObjectPtr<const UAblAbilityContext>& Context) const
{
	Super::OnTaskStart(Context);

	const UAnimationAsset* AnimationAsset = m_UseDynamicAnimationAsset ? Context->GetAbility()->OnGetAnimationAssetBP(Context.Get(), m_EventName) : m_AnimationAsset;

	if (!AnimationAsset)
	{
		UE_LOG(LogAble, Warning, TEXT("No Animation set for PlayAnimationTask in Ability [%s]"), *Context->GetAbility()->GetDisplayName());
		return;
	}

	TArray<TWeakObjectPtr<AActor>> TargetArray;
	GetActorsForTask(Context, TargetArray);

	UAblPlayAnimationTaskScratchPad* ScratchPad = CastChecked<UAblPlayAnimationTaskScratchPad>(Context->GetScratchPadForTask(this));

	float PlayRate = m_PlayRate * (m_ScaleWithAbilityPlayRate ? Context->GetAbility()->GetPlayRate(Context.Get()) : 1.0f);

	TArray<UActorComponent*> SkeletalComponents;
	for (TWeakObjectPtr<AActor>& Target : TargetArray)
	{
		if (Target.IsValid())
		{
			SkeletalComponents = Target->GetComponentsByClass(USkeletalMeshComponent::StaticClass());
			for (UActorComponent* SkeletalComponent : SkeletalComponents)
			{
				PlayAnimation(AnimationAsset, *Target.Get(), *ScratchPad, *Cast<USkeletalMeshComponent>(SkeletalComponent), PlayRate);
			}
		}
	}
}

void UAblPlayAnimationTask::OnTaskEnd(const TWeakObjectPtr<const UAblAbilityContext>& Context, const EAblAbilityTaskResult result) const
{
	Super::OnTaskEnd(Context, result);

	if (!Context.IsValid())
	{
		return;
	}

	UAblPlayAnimationTaskScratchPad* ScratchPad = CastChecked<UAblPlayAnimationTaskScratchPad>(Context->GetScratchPadForTask(this));
	check(ScratchPad);
	
	// Reset any Single Node instances that were previous AnimBlueprint mode.
	for (TWeakObjectPtr<USkeletalMeshComponent>& SkeletalComponent : ScratchPad->SingleNodeSkeletalComponents)
	{
		SkeletalComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);

		// If we're in Animation Node mode, and our Ability was interrupted - tell the Animation Node.
		if (m_StopAnimationOnInterrupt && result == EAblAbilityTaskResult::Interrupted )
		{
			switch (m_AnimationMode)
			{
			case EAblPlayAnimationTaskAnimMode::AbilityAnimationNode:
			{
				if (FAnimNode_AbilityAnimPlayer* AnimPlayer = GetAbilityAnimGraphNode(SkeletalComponent.Get()))
				{
					AnimPlayer->OnAbilityInterrupted(m_ClearQueuedAnimationOnInterrupt);
				}
			}
			break;
			case EAblPlayAnimationTaskAnimMode::DynamicMontage:
			{
				if (UAnimInstance* Instance = SkeletalComponent->GetAnimInstance())
				{
					Instance->Montage_Stop(m_DynamicMontageBlend.m_BlendOut);
				}
			}
			break;
			case EAblPlayAnimationTaskAnimMode::SingleNode:
			{
				if (UAnimSingleNodeInstance* SingleNode = SkeletalComponent->GetSingleNodeInstance())
				{
					SingleNode->StopAnim();
				}
			}
			break;
			default:
				checkNoEntry();
				break;
			}

		}
	}
}

float UAblPlayAnimationTask::GetEndTime() const
{
	if (const UAnimSequenceBase* Sequence = Cast<UAnimSequenceBase>(m_AnimationAsset))
	{
		return GetStartTime() + (const_cast<UAnimSequenceBase*>(Sequence)->GetPlayLength() * (1.0f/m_PlayRate)); // No idea why this is non-const.
	}
	else if (const UAnimMontage* Montage = Cast<UAnimMontage>(m_AnimationAsset))
	{
		return GetStartTime() + (const_cast<UAnimMontage*>(Montage)->GetPlayLength() * (1.0f/m_PlayRate)); 
	}

	return GetStartTime() + 1.0f; // Useless magic number just for worst case display purposes.
}

UAblAbilityTaskScratchPad* UAblPlayAnimationTask::CreateScratchPad(const TWeakObjectPtr<UAblAbilityContext>& Context) const
{
	return NewObject<UAblPlayAnimationTaskScratchPad>(Context.Get());
}

TStatId UAblPlayAnimationTask::GetStatId() const
{
	 RETURN_QUICK_DECLARE_CYCLE_STAT(UAblPlayAnimationTask, STATGROUP_Able);
}

void UAblPlayAnimationTask::SetAnimationAsset(const UAnimationAsset* Animation)
{
	check(Animation->IsA<UAnimSequenceBase>() || Animation->IsA<UAnimMontage>());
	m_AnimationAsset = Animation;
}

void UAblPlayAnimationTask::PlayAnimation(const UAnimationAsset* AnimationAsset, AActor& TargetActor, UAblPlayAnimationTaskScratchPad& ScratchPad, USkeletalMeshComponent& SkeletalMeshComponent, float PlayRate) const
{
	switch (m_AnimationMode.GetValue())
	{
		case EAblPlayAnimationTaskAnimMode::SingleNode:
		{
			// Make a note of these so we can reset to Animation Blueprint mode.
			if (SkeletalMeshComponent.GetAnimationMode() == EAnimationMode::AnimationBlueprint)
			{
				ScratchPad.SingleNodeSkeletalComponents.Add(&SkeletalMeshComponent);
			}

			if (const UAnimMontage* MontageAsset = Cast<UAnimMontage>(AnimationAsset))
			{
				if (UAnimInstance* Instance = SkeletalMeshComponent.GetAnimInstance())
				{
#if !(UE_BUILD_SHIPPING)
					if (IsVerbose())
					{
						PrintVerbose(FString::Printf(TEXT("Playing Single Node Montage %s on Target %s"), *MontageAsset->GetName(), *TargetActor.GetName()));
					}
#endif
					Instance->Montage_Play(const_cast<UAnimMontage*>(MontageAsset), PlayRate);
				}
			}
			else if (UAnimSingleNodeInstance* SingleNode = SkeletalMeshComponent.GetSingleNodeInstance()) // See if we already have an instance instantiated.
			{
#if !(UE_BUILD_SHIPPING)
				if (IsVerbose())
				{
					PrintVerbose(FString::Printf(TEXT("Playing Single Node Animation %s on Target %s"), *AnimationAsset->GetName(), *TargetActor.GetName()));
				}
#endif
				SingleNode->SetAnimationAsset(const_cast<UAnimationAsset*>(AnimationAsset), m_Loop, PlayRate);
				SingleNode->PlayAnim();
			}
			else // Nope, start a new one.
			{
#if !(UE_BUILD_SHIPPING)
				if (IsVerbose())
				{
					PrintVerbose(FString::Printf(TEXT("Playing Single Node Animation %s on Target %s"), *AnimationAsset->GetName(), *TargetActor.GetName()));
				}
#endif
				SkeletalMeshComponent.SetAnimationMode(EAnimationMode::AnimationSingleNode);
				SkeletalMeshComponent.SetAnimation(const_cast<UAnimationAsset*>(AnimationAsset));
				SkeletalMeshComponent.SetPlayRate(PlayRate);
				SkeletalMeshComponent.Play(m_Loop);
			}
		}
		break;
		case EAblPlayAnimationTaskAnimMode::AbilityAnimationNode:
		{
			if (SkeletalMeshComponent.GetAnimationMode() != EAnimationMode::AnimationBlueprint)
			{
				SkeletalMeshComponent.SetAnimationMode(EAnimationMode::AnimationBlueprint);
			}

			if (UAnimInstance* Instance = SkeletalMeshComponent.GetAnimInstance())
			{
				const UAnimSequence* AnimationSequence = Cast<UAnimSequence>(AnimationAsset);
				if (AnimationSequence)
				{
					if (FAnimNode_AbilityAnimPlayer* AbilityPlayerNode = GetAbilityAnimGraphNode(&SkeletalMeshComponent))
					{
#if !(UE_BUILD_SHIPPING)
						if (IsVerbose())
						{
							PrintVerbose(FString::Printf(TEXT("Playing Animation %s on Target %s using Ability Animation Node"), *AnimationAsset->GetName(), *TargetActor.GetName()));
						}
#endif

						AbilityPlayerNode->PlayAnimationSequence(AnimationSequence, PlayRate, m_BlendIn, m_BlendOut);

						if (UAblAbilityComponent* AbilityComponent = TargetActor.FindComponentByClass<UAblAbilityComponent>())
						{
							ScratchPad.AbilityComponents.Add(AbilityComponent);

							AbilityComponent->SetAbilityAnimationNode(AbilityPlayerNode);
						}
					}
					else
					{
						UE_LOG(LogAble, Warning, TEXT("Failed to find Ability Animation Node using State Machine Name %s and Node Name %s"), *m_StateMachineName.ToString(), *m_AbilityStateName.ToString());
					}
				}
			}
		}
		break;
		case EAblPlayAnimationTaskAnimMode::DynamicMontage:
		{
			if (SkeletalMeshComponent.GetAnimationMode() != EAnimationMode::AnimationBlueprint)
			{
				SkeletalMeshComponent.SetAnimationMode(EAnimationMode::AnimationBlueprint);
			}

			if (UAnimInstance* Instance = SkeletalMeshComponent.GetAnimInstance())
			{
				if (const UAnimMontage* MontageAsset = Cast<UAnimMontage>(AnimationAsset))
				{
#if !(UE_BUILD_SHIPPING)
					if (IsVerbose())
					{
						PrintVerbose(FString::Printf(TEXT("Playing Montage Animation %s on Target %s using Dynamic Montage"), *MontageAsset->GetName(), *TargetActor.GetName()));
					}
#endif
					Instance->Montage_Play(const_cast<UAnimMontage*>(MontageAsset), PlayRate);
				}
				else if (const UAnimSequenceBase* SequenceAsset = Cast<UAnimSequenceBase>(AnimationAsset))
				{
#if !(UE_BUILD_SHIPPING)
					if (IsVerbose())
					{
						PrintVerbose(FString::Printf(TEXT("Playing Slot Animation %s on Target %s on Slot %s using Dynamic Montage"), *SequenceAsset->GetName(), *TargetActor.GetName(), *m_SlotName.ToString()));
					}
#endif
					Instance->PlaySlotAnimationAsDynamicMontage(const_cast<UAnimSequenceBase*>(SequenceAsset), m_SlotName, m_DynamicMontageBlend.m_BlendIn, m_DynamicMontageBlend.m_BlendOut, PlayRate);
				}
			}
		}
		break;
		default:
		{
			checkNoEntry();
		}
		break;
	}

	if (m_StopAnimationOnInterrupt)
	{
		ScratchPad.SingleNodeSkeletalComponents.Add(&SkeletalMeshComponent);
	}
}

FAnimNode_AbilityAnimPlayer* UAblPlayAnimationTask::GetAbilityAnimGraphNode(USkeletalMeshComponent* MeshComponent) const
{
	if (UAnimInstance* Instance = MeshComponent->GetAnimInstance())
	{
		FAnimInstanceProxy InstanceProxy(Instance);

		FAnimNode_StateMachine* StateMachineNode = InstanceProxy.GetStateMachineInstanceFromName(m_StateMachineName);
		if (StateMachineNode)
		{
			const FBakedAnimationStateMachine* BakedStateMachine = InstanceProxy.GetMachineDescription(InstanceProxy.GetAnimClassInterface(), StateMachineNode);

			if (BakedStateMachine)
			{
				for (const FBakedAnimationState& State : BakedStateMachine->States)
				{
					if (State.StateName == m_AbilityStateName)
					{
						for (const int32& PlayerNodeIndex : State.PlayerNodeIndices)
						{
							FAnimNode_AbilityAnimPlayer* AbilityPlayerNode = (FAnimNode_AbilityAnimPlayer*)InstanceProxy.GetNodeFromIndexUntyped(PlayerNodeIndex, FAnimNode_AbilityAnimPlayer::StaticStruct());
							if (AbilityPlayerNode)
							{
								return AbilityPlayerNode;
							}
						}
					}
				}
			}
		}
	}

	return nullptr;
}

void UAblPlayAnimationTask::OnAbilityTimeSet(const TWeakObjectPtr<const UAblAbilityContext>& Context)
{
	UAblPlayAnimationTaskScratchPad* ScratchPad = CastChecked<UAblPlayAnimationTaskScratchPad>(Context->GetScratchPadForTask(this));
	check(ScratchPad)

	switch (m_AnimationMode.GetValue())
	{
	case EAblPlayAnimationTaskAnimMode::AbilityAnimationNode:
	{
		// Reset any Single Node instances that were previous AnimBlueprint mode.
		for (TWeakObjectPtr<USkeletalMeshComponent>& SkeletalComponent : ScratchPad->SingleNodeSkeletalComponents)
		{
			if (FAnimNode_AbilityAnimPlayer* AbilityNode = GetAbilityAnimGraphNode(SkeletalComponent.Get()))
			{
				AbilityNode->SetAnimationTime(Context->GetCurrentTime() - GetStartTime());
			}
		}
	}
	case EAblPlayAnimationTaskAnimMode::SingleNode:
	{
		for (TWeakObjectPtr<USkeletalMeshComponent>& SkeletalComponent : ScratchPad->SingleNodeSkeletalComponents)
		{
			if (UAnimSingleNodeInstance* SingleNode = SkeletalComponent->GetSingleNodeInstance())
			{
				SingleNode->SetPosition(Context->GetCurrentTime() - GetStartTime());
			}
		}
	}
	break;
	default:
		break;
	}
}

#if WITH_EDITOR

FText UAblPlayAnimationTask::GetDescriptiveTaskName() const
{
	const FText FormatText = LOCTEXT("AblPlayAnimationTaskFormat", "{0}: {1}");
	FString AnimationName = TEXT("<null>");
	if (m_AnimationAsset)
	{
		AnimationName = m_AnimationAsset->GetName();
	}
	else if (m_UseDynamicAnimationAsset)
	{
		AnimationName = TEXT("Dynamic");
	}

	return FText::FormatOrdered(FormatText, GetTaskName(), FText::FromString(AnimationName));
}

#endif


#undef LOCTEXT_NAMESPACE

