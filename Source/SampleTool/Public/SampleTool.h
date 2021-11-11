// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;
class FUICommandList;

class FSampleToolModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	
private:

	void AddToolbarEntries(FToolBarBuilder& Builder);
	void AddMenuEntries(FMenuBuilder& Builder);
	void AddMenubarEntries(FMenuBarBuilder& Builder);

	void AddPullDown(FMenuBuilder& Builder);
	void AddSubMenu(FMenuBuilder& Builder);

	TSharedRef<FExtender> AddContextMenuItem(const TSharedRef<FUICommandList> InCommandList, const TArray<AActor*> InActor);
	void EditorPrint(FString MyString);
	void AddSelectedActorButton(FMenuBuilder& Builder);
	FDelegateHandle DelHandle;

	TSharedRef<FExtender> AddAssetBrowserContextMenuItem(const TArray<FString>& SelectedPaths);

private:
	TSharedPtr<FUICommandList> PluginCommands;
};
