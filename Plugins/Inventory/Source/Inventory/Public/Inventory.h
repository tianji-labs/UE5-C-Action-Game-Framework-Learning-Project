// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

//声明一个日志类别LogInventory
DECLARE_LOG_CATEGORY_EXTERN(LogInventory, Log, All);

class FInventoryModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
