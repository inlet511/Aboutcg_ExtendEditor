// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SampleToolStyle.h"

class FSampleToolCommands : public TCommands<FSampleToolCommands>
{
public:

	FSampleToolCommands()
		: TCommands<FSampleToolCommands>(TEXT("SampleTool"), NSLOCTEXT("Contexts", "SampleTool", "SampleTool Plugin"), NAME_None, FSampleToolStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
