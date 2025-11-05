// Fill out your copyright notice in the Description page of Project Settings.
//DRONEMANAGEMENTSYS_API
#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

class FSQLiteDatabase;
class FSQLitePreparedStatement;

#include "DatabaseSubsystem.generated.h"

UCLASS()
class DRONEMANAGEMENTSYS_API UDatabaseSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	bool EnsureSchema();

	bool InsertUser(const FString& Name, int32 Score, int64& OutRowId);
	bool UpdateUserScore(int64 Id, int32 NewScore);
	TArray<TTuple<int64, FString, int32>> GetAllUsers();

private:
	FSQLiteDatabase* Db = nullptr;

	FSQLitePreparedStatement* StmtInsertUser = nullptr;
	FSQLitePreparedStatement* StmtUpdateUser = nullptr;
	FSQLitePreparedStatement* StmtSelectAll = nullptr;

	bool PrepareStatements();
	void DestroyStatements();
};
