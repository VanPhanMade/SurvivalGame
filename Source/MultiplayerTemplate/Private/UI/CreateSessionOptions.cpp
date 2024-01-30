// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CreateSessionOptions.h"
#include "Components/Button.h"

void UCreateSessionOptions::MenuInit()
{

}

void UCreateSessionOptions::MenuTearDown()
{

}

bool UCreateSessionOptions::Initialize()
{
    if(!Super::Initialize()) return false;

    if(BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &ThisClass::BackButtonClicked);
    }

    return true;
}

void UCreateSessionOptions::BackButtonClicked()
{
    OnBackButtonCreateSessionClicked.Broadcast();
}
