// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NetworkManager.generated.h"





UCLASS()
class DRONEMANAGEMENTSYS_API ANetworkManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ANetworkManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	
	void DNSReolve(const FString& Hostname);
	void HttpGet(const FString& URL);
	void FTP();
	//void SMTP();

	UPROPERTY(EditAnywhere, Category = "Network")
	FString HostToResolve = TEXT("jsonplaceholder.typicode.com");
	
	UPROPERTY(EditAnywhere, Category = "Network")
	FString HttpUrl = TEXT("https://jsonplaceholder.typicode.com/todos/1");

	UPROPERTY(EditAnywhere, Category = "FTP")
	FString FTPServer;

	UPROPERTY(EditAnywhere, Category = "FTP")
	FString LocalFileToUpload;

	UPROPERTY(EditAnywhere, Category = "FTP")
	FString FTPUsername;

	UPROPERTY(EditAnywhere, Category = "FTP")
	FString FTPPassword;

//	UPROPERTY(EditAnywhere, Category = "SMTP")
//	FString SMTPServer;
//
//	UPROPERTY(EditAnywhere, Category = "SMTP")
//	FString SenderEmail;
//
//	UPROPERTY(EditAnywhere, Category = "SMTP")
//	FString SenderPassword;
//
//	UPROPERTY(EditAnywhere, Category = "SMTP")
//	FString RecipientEmail;
//
//	UPROPERTY(EditAnywhere, Category = "SMTP")
//	FString EmailSubject;
//
//	UPROPERTY(EditAnywhere, Category = "SMTP")
//	FString EmailBody;
};
