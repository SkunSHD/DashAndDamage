// Fill out your copyright notice in the Description page of Project Settings.


#include "DDAI.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"


ADDAI::ADDAI()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Overlap);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Overlap);
}