// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"



UCLASS()
class CPPTPS_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	float currTime = 0;
	float createTime = 0;
	float min = 1;
	float max = 5;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemy> enemyFactory;


	UPROPERTY(EditAnywhere)
	TArray<class AEnemy*> enemyArray;

public:
	UFUNCTION()
	void AddEnemy(class AEnemy* e);

};
