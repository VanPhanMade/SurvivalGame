// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServerSearchList.h"
#include "Components/Button.h"

bool UServerSearchList::Initialize()
{
    if(!Super::Initialize()) return false;

    if(BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &ThisClass::BackButtonClicked);
    }

    return true;
}

void UServerSearchList::BackButtonClicked()
{
    OnBackButtonServerSearchListClicked.Broadcast();
}
