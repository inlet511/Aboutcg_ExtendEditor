// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SampleTool.h"
#include "SampleToolStyle.h"
#include "SampleToolCommands.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "LevelEditor.h"
#include "SImage.h"
#include "SWidget.h"
#include "ContentBrowserModule.h"

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
		
	//获取LevelEditor模块
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	// 给某个菜单添加子内容
	{
		TSharedRef<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FSampleToolModule::AddMenuEntries));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	// 给工具栏添加入口
	{
		TSharedRef<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FSampleToolModule::AddToolbarEntries));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	// 给菜单栏添加入口
	{
		TSharedRef<FExtender> MenuBarExtender = MakeShareable(new FExtender());
		MenuBarExtender->AddMenuBarExtension("Help", EExtensionHook::After, PluginCommands, FMenuBarExtensionDelegate::CreateRaw(this, &FSampleToolModule::AddMenubarEntries));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuBarExtender);
	}

	// 给场景中或者Outliner中右键菜单添加入口
	{
		auto& contextMenuArray = LevelEditorModule.GetAllLevelViewportContextMenuExtenders();
		contextMenuArray.Add(FLevelEditorModule::FLevelViewportMenuExtender_SelectedActors::CreateRaw(this, &FSampleToolModule::AddContextMenuItem));
		DelHandle = contextMenuArray.Last().GetHandle(); // 储存代理的方法
	}

	// 给资源管理器右键添加入口
	{
		//注意这里用的管理模块不是LevelEditorModule了，而是ContentBrowserModule, 需要在.cs文件中添加ContentBrowser模块
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		auto& PathViewAMenuArray = ContentBrowserModule.GetAllPathViewContextMenuExtenders();
		PathViewAMenuArray.Add(FContentBrowserMenuExtender_SelectedPaths::CreateRaw(this, &FSampleToolModule::AddAssetBrowserContextMenuItem));
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

void FSampleToolModule::AddMenuEntries(FMenuBuilder& Builder)
{
	Builder.BeginSection(TEXT("SampleSection"));
	Builder.AddMenuEntry(FSampleToolCommands::Get().PluginAction);
	Builder.AddMenuEntry(FSampleToolCommands::Get().PluginAction);
	Builder.AddMenuEntry(FSampleToolCommands::Get().PluginAction);
	Builder.AddMenuEntry(FSampleToolCommands::Get().PluginAction);
	Builder.AddMenuEntry(FSampleToolCommands::Get().PluginAction);
	Builder.EndSection();
}

void FSampleToolModule::AddMenubarEntries(FMenuBarBuilder& Builder)
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


TSharedRef<FExtender> FSampleToolModule::AddContextMenuItem(const TSharedRef<FUICommandList> InCommandList, const TArray<AActor*> InActor)
{	
	TSharedRef<FExtender> MenuExtender = MakeShareable(new FExtender());
	EditorPrint(FString::Printf(TEXT("Actor Number = %d"), InActor.Num()));
	if(InActor.Num() >0)
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		TSharedRef<FUICommandList> LevelCommand = LevelEditorModule.GetGlobalLevelEditorActions();

		MenuExtender->AddMenuExtension("NewFolder", EExtensionHook::After, LevelCommand, FMenuExtensionDelegate::CreateRaw(this, &FSampleToolModule::AddSelectedActorButton));
	}
	return MenuExtender;
}

void FSampleToolModule::EditorPrint(FString MyString)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, MyString);
	}
}

void FSampleToolModule::AddSelectedActorButton(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FSampleToolCommands::Get().PluginAction);
}

TSharedRef<FExtender> FSampleToolModule::AddAssetBrowserContextMenuItem(const TArray<FString>& SelectedPaths)
{
	TSharedRef<FExtender> MenuExtender = MakeShareable(new FExtender());
	EditorPrint(FString::Printf(TEXT("Actor Number = %d"), SelectedPaths.Num()));
	if (SelectedPaths.Num() > 0)
	{
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

		MenuExtender->AddMenuExtension("ActorControl", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FSampleToolModule::AddSelectedActorButton));
	}

	for(auto& path : SelectedPaths)
	{
		EditorPrint(path);
	}

	return MenuExtender;
}

void FSampleToolModule::AddToolbarEntries(FToolBarBuilder& Builder)
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