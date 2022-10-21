// Fill out your copyright notice in the Description page of Project Settings.

#include "Tests/JsonUtils.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonReader.h"
#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"

DEFINE_LOG_CATEGORY_STATIC(LogJsonUtils, All, All);

using namespace UTPDGame::Test;
bool UTPDGame::Test::JsonUtils::WriteInputData(const FString& FileName, const FInputData& InputData)
{
    TSharedPtr<FJsonObject> MainJsonObject = FJsonObjectConverter::UStructToJsonObject(InputData);
    if (!MainJsonObject.IsValid()) return false;

    FString OutputString;
    TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);

    if (!FJsonSerializer::Serialize(MainJsonObject.ToSharedRef(), JsonWriter))
    {
        UE_LOG(LogJsonUtils, Error, TEXT("JSON serialization error"));
        return false;
    }
    if (!JsonWriter->Close())
    {
        UE_LOG(LogJsonUtils, Error, TEXT("JSON writer close error"));
        return false;
    }
    if (!FFileHelper::SaveStringToFile(OutputString, *FileName))
    {
        UE_LOG(LogJsonUtils, Error, TEXT("JSON writer save error"));
        return false;
    }

    return true;
}

bool UTPDGame::Test::JsonUtils::ReadInputData(const FString& FileName, FInputData& InputData)
{
    TSharedPtr<FJsonObject> MainJsonObject = MakeShareable(new FJsonObject());

    FString FileContentString;
    if (!FFileHelper::LoadFileToString(FileContentString, *FileName))
    {
        UE_LOG(LogJsonUtils, Error, TEXT("JSON load error"));
        return false;
    }

    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(FileContentString);
    if (!FJsonSerializer::Deserialize(JsonReader, MainJsonObject))
    {
        UE_LOG(LogJsonUtils, Error, TEXT("JSON deserializaton error"));
        return false;
    }

        return FJsonObjectConverter::JsonObjectToUStruct(MainJsonObject.ToSharedRef(), &InputData, 0, 0);
}
