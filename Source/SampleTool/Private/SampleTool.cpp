// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SampleTool.h"
#include "SampleToolStyle.h"
#include "SampleToolCommands.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "LevelEditor.h"
#include "SImage.h"
#include "SWidget.h"

static const FName SampleToolTabName("SampleTool");

#define LOCTEXT_NAMESPACE "FSampleToolModule"


TSharedRef<SWidget> CreateWidgetContent(TSharedRef<FUICommandList> InCommandList)
{
	FMenuBuilder MenuBuilder(true, InCommandList);
	MenuBuilder.AddMenuEntry(FSampleToolCommands::Get().PluginAction);
	return MenuBuilder.MakeWidget();
}


void FSampleToolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSampleToolStyle::Initialize();
	FSampleToolStyle::ReloadTextures();

	FSampleToolCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSampleToolCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FSampleToolModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FSampleToolModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FSampleToolModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	{
		TSharedPtr<FExtender> MenuBarExtender = MakeShareable(new FExtender());
		MenuBarExtender->AddMenuBarExtension("Help", EExtensionHook::After, PluginCommands, FMenuBarExtensionDelegate::CreateRaw(this, &FSampleToolModule::AddMenubarExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuBarExtender);
	}
}

void FSampleToolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSampleToolStyle::Shutdown();

	FSampleToolCommands::Unregister();
}

void FSampleToolModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FSampleToolModule::PluginButtonClicked()")),
							FText::FromString(TEXT("SampleTool.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FSampleToolModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.BeginSection(TEXT("SampleSection"));
	Builder.AddMenuEntry(FSampleToolCommands::Get().PluginAction);
	Builder.AddMenuEntry(FSampleToolCommands::Get().PluginAction);
	Builder.AddMenuEntry(FSampleToolCommands::Get().PluginAction);
	Builder.AddMenuEntry(FSampleToolCommands::Get().PluginAction);
	Builder.AddMenuEntry(FSampleToolCommands::Get().PluginAction);
	Builder.EndSection();
}

void FSampleToolModule::AddMenubarExtension(FMenuBarBuilder& Builder)
{

	Builder.AddPullDownMenu(
		LOCTEXT("MenuName", "Ken's"),
		LOCTEXT("ButtonName", "I am button"),
		FNewMenuDelegate::CreateRaw(this, &FSampleToolModule::AddPullDown),
		"MyHookPoint"
	);

}

void FSampleToolModule::AddPullDown(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FSampleToolCommands::Get().PluginAction);
	Builder.AddMenuEntry(FSampleToolCommands::Get().PluginAction);
	Builder.AddMenuSeparator();
	Builder.AddSubMenu(
		LOCTEXT("SubMenuName", "MoreContent"),
		LOCTEXT("Tooltip", "This is a submenu"),
		FNewMenuDelegate::CreateRaw(this,&FSampleToolModule::AddSubMenu)
	);
	Builder.AddMenuEntry(FSampleToolCommands::Get().PluginAction);


}

void FSampleToolModule::AddSubMenu(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FSampleToolCommands::Get().PluginAction);
	Builder.AddMenuEntry(FSampleToolCommands::Get().PluginAction);
	Builder.AddWidget(
		SNew(SImage),
		LOCTEXT("ImageName","An Image")
	);
	Builder.AddEditableText(
		LOCTEXT("LabelName", "NameField"),
		LOCTEXT("TextTip", "Some Text as tip"),
		FSlateIcon(),
		LOCTEXT("Text_to_Edit","Here are some important texts")
	);

	Builder.AddSearchWidget();

	Builder.AddWrapperSubMenu(
		LOCTEXT("WrapperName", "NameField"),
		LOCTEXT("WrapperTip", "Some Text as tip"),
		//FOnGetContent::CreateRaw(this, &FSampleToolModule::CreateWidgetContent),
		FOnGetContent::CreateStatic(&CreateWidgetContent,PluginCommands.ToSharedRef()),
		FSlateIcon()
	);
}


void FSampleToolModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	//Builder.AddToolBarButton(FSampleToolCommands::Get().PluginAction);

	Builder.AddComboButton(
		FUIAction(),
		FOnGetContent::CreateStatic(&CreateWidgetContent, PluginCommands.ToSharedRef()),
		LOCTEXT("ButtonName","SampleButton"),
		LOCTEXT("ButtonTips", "SampleTips"),
		FSlateIcon()
	);


}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSampleToolModule, SampleTool)