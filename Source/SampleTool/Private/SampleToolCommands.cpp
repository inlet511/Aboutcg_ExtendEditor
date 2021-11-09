// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SampleToolCommands.h"

#define LOCTEXT_NAMESPACE "FSampleToolModule"

void FSampleToolCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "SampleTool", "Execute SampleTool action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
