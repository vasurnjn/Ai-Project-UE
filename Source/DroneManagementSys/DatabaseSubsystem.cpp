






#include "DatabaseSubsystem.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "SQLiteDatabase.h"                
#include "SQLitePreparedStatement.h"     

void UDatabaseSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const FString DbDir = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Databases"));
	IPlatformFile& PF = FPlatformFileManager::Get().GetPlatformFile();
	if (!PF.DirectoryExists(*DbDir)) { PF.CreateDirectoryTree(*DbDir); }

	const FString DbPath = FPaths::Combine(DbDir, TEXT("game.db"));

	Db = new FSQLiteDatabase();
	if (!Db->Open(*DbPath, ESQLiteDatabaseOpenMode::ReadWriteCreate))
	{
		UE_LOG(LogTemp, Error, TEXT("SQLite open failed: %s"), *DbPath);
		delete Db; Db = nullptr;
		return;
	}

	if (!EnsureSchema() || !PrepareStatements())
	{
		UE_LOG(LogTemp, Error, TEXT("SQLite init failed."));
	}
}

void UDatabaseSubsystem::Deinitialize()
{
	DestroyStatements();

	if (Db)
	{
		Db->Close();       
		delete Db; Db = nullptr;
	}
	Super::Deinitialize();
}

bool UDatabaseSubsystem::EnsureSchema()
{
	if (!Db) return false;

	const TCHAR* SQL =
		TEXT("CREATE TABLE IF NOT EXISTS Users (")
		TEXT("  id    INTEGER PRIMARY KEY AUTOINCREMENT,")
		TEXT("  name  TEXT NOT NULL,")
		TEXT("  score INTEGER NOT NULL DEFAULT 0")
		TEXT(");");


	return Db->Execute(SQL);
}

bool UDatabaseSubsystem::PrepareStatements()
{
	if (!Db) return false;

	StmtInsertUser = new FSQLitePreparedStatement();
	if (!StmtInsertUser->Create(*Db,
		TEXT("INSERT INTO Users(name, score) VALUES(?1, ?2);"),
		ESQLitePreparedStatementFlags::Persistent))
	{
		return false;
	}

	StmtUpdateUser = new FSQLitePreparedStatement();
	if (!StmtUpdateUser->Create(*Db,
		TEXT("UPDATE Users SET score = ?1 WHERE id = ?2;"),
		ESQLitePreparedStatementFlags::Persistent))
	{
		return false;
	}

	StmtSelectAll = new FSQLitePreparedStatement();
	if (!StmtSelectAll->Create(*Db,
		TEXT("SELECT id, name, score FROM Users ORDER BY id;"),
		ESQLitePreparedStatementFlags::Persistent))
	{
		return false;
	}

	return true;
}

void UDatabaseSubsystem::DestroyStatements()
{
	if (StmtInsertUser) { StmtInsertUser->Destroy(); delete StmtInsertUser; StmtInsertUser = nullptr; }
	if (StmtUpdateUser) { StmtUpdateUser->Destroy(); delete StmtUpdateUser; StmtUpdateUser = nullptr; }
	if (StmtSelectAll) { StmtSelectAll->Destroy();  delete StmtSelectAll;  StmtSelectAll = nullptr; }
}

bool UDatabaseSubsystem::InsertUser(const FString& Name, int32 Score, int64& OutRowId)
{
	if (!Db || !StmtInsertUser) return false;

	StmtInsertUser->Reset();
	bool ok = StmtInsertUser->SetBindingValueByIndex(1, Name)
		&& StmtInsertUser->SetBindingValueByIndex(2, Score);


	if (ok && StmtInsertUser->Execute())
	{
		OutRowId = Db->GetLastInsertRowId();
		return true;
	}
	return false;
}

bool UDatabaseSubsystem::UpdateUserScore(int64 Id, int32 NewScore)
{
	if (!Db || !StmtUpdateUser) return false;

	StmtUpdateUser->Reset();
	bool ok = StmtUpdateUser->SetBindingValueByIndex(1, NewScore)
		&& StmtUpdateUser->SetBindingValueByIndex(2, Id);

	return ok && StmtUpdateUser->Execute();
}

TArray<TTuple<int64, FString, int32>> UDatabaseSubsystem::GetAllUsers()
{
	TArray<TTuple<int64, FString, int32>> Rows;
	if (!Db || !StmtSelectAll) return Rows;

	StmtSelectAll->Reset();

	while (StmtSelectAll->Step() == ESQLitePreparedStatementStepResult::Row)
	{
		int64 Id;    FString Name; int32 Score;
		StmtSelectAll->GetColumnValueByIndex(0, Id);
		StmtSelectAll->GetColumnValueByIndex(1, Name);
		StmtSelectAll->GetColumnValueByIndex(2, Score);
		Rows.Emplace(Id, Name, Score);
	}
	return Rows;
}
