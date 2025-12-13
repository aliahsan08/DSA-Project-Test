#include "EndLevelWidget.h"
#include "dsaprojectGameMode.h"
#include "DSAManagerComponent.h"
#include "DSAGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UEndLevelWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Btn_NextLevel) Btn_NextLevel->OnClicked.AddDynamic(this, &UEndLevelWidget::OnNextLevelClicked);

    // Get GameMode Data
    AdsaprojectGameMode* GM = Cast<AdsaprojectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GM)
    {
        // 1. Show Current Score
        if (Txt_Score)
        {
            Txt_Score->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), GM->CurrentScore)));
        }

        // 2. Show High Scores (Top 3)
        if (GM->DSAManager && Txt_HighScores)
        {
            FString ScoreList = TEXT("Top Scores:\n");

            // DSA: Iterate through the sorted TArray
            for (int32 i = 0; i < FMath::Min(3, GM->DSAManager->Scoreboard.Num()); i++)
            {
                FLevelScore Entry = GM->DSAManager->Scoreboard[i];
                ScoreList += FString::Printf(TEXT("%d. %s - %d\n"), i + 1, *Entry.PlayerName, Entry.Score);
            }
            Txt_HighScores->SetText(FText::FromString(ScoreList));
        }
    }
}

void UEndLevelWidget::OnNextLevelClicked()
{
    UDSAGameInstance* GI = Cast<UDSAGameInstance>(GetGameInstance());
    if (GI)
    {
        GI->OpenNextLevel();
    }
}