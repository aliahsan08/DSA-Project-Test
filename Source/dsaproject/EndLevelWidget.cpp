#include "EndLevelWidget.h"
#include "dsaprojectGameMode.h"
#include "DSAManagerComponent.h"
#include "DSAGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UEndLevelWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Btn_NextLevel) Btn_NextLevel->OnClicked.AddDynamic(this, &UEndLevelWidget::OnNextLevelClicked);

    // Bind Save Button
    if (Btn_Save) Btn_Save->OnClicked.AddDynamic(this, &UEndLevelWidget::OnSaveClicked);

    AdsaprojectGameMode* GM = Cast<AdsaprojectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GM)
    {
        if (Txt_Score)
        {
            Txt_Score->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), GM->CurrentScore)));
        }

        if (GM->DSAManager && Txt_HighScores)
        {
            // Filter scores for the level that was just completed
            const int32 LevelToShow = GM->CompletedLevelIndex;

            TArray<FLevelScore> Filtered;
            for (const FLevelScore& Entry : GM->DSAManager->Scoreboard)
            {
                if (Entry.LevelIndex == LevelToShow)
                {
                    Filtered.Add(Entry);
                }
            }

            FString ScoreList = TEXT("Top Scores (This Level):\n");
            const int32 MaxToShow = FMath::Min(3, Filtered.Num());
            for (int32 i = 0; i < MaxToShow; i++)
            {
                const FLevelScore& Entry = Filtered[i];
                ScoreList += FString::Printf(TEXT("%d. %s - %d\n"), i + 1, *Entry.PlayerName, Entry.Score);
            }

            if (MaxToShow == 0)
            {
                ScoreList += TEXT("No previous scores yet.\n");
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

void UEndLevelWidget::OnSaveClicked()
{
    UDSAGameInstance* GI = Cast<UDSAGameInstance>(GetGameInstance());
    if (GI)
    {
        // Saves current state (Next Level Index) to disk
        GI->SaveState();

        if (Btn_Save) Btn_Save->SetIsEnabled(false); // Visual feedback
    }
}