// Fill out your copyright notice in the Description page of Project Settings.


#include "GASAttributeSet.h"


// Later I will add some multiplayer features to the game .

void UGASAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(UGASAttributeSet, Health);
    DOREPLIFETIME(UGASAttributeSet, Ammu);
    DOREPLIFETIME(UGASAttributeSet, FlyingEnergy);
    
}
