// Copyright (c) 2016 - 2018 Extra Life Studios, LLC. All rights reserved.

#pragma once

#include "Modules/ModuleManager.h"

class IAbleCore : public IModuleInterface
{

public:

	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline IAbleCore& Get()
	{
		return FModuleManager::LoadModuleChecked< IAbleCore >( "AbleCore" );
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded( "AbleCore" );
	}
};

