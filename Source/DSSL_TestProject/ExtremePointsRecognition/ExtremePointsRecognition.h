// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ExtremePointsRecognition.generated.h"

/**
 * Library for recognition screen-relative extreme points of mesh.
 */
UCLASS()
class DSSL_TESTPROJECT_API UExtremePointsRecognition : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Get world-space coordinates with all vertices of skeletal mesh.
	 * @param OutVertices Array of world coordinates of each vertex.
	 * @param MeshComponent Mesh to use
	 * @param LODIndex
	 * @return True if success
	 */
	UFUNCTION(BlueprintCallable, Category = "Extreme Points Recognition")
	static bool GetSkeletalMeshVertices(TArray<FVector3f>& OutVertices, USkeletalMeshComponent* MeshComponent, int32 LODIndex);

	/**
	 * Find extreme points and project it to screen.
	 * @param LocalPlayerController 
	 * @param Vertices All vertices to check
	 * @return Array of four screen-center-relative 2D points with extreme points (left, right, top, bottom)
	 */
	UFUNCTION(BlueprintCallable, Category = "Extreme Points Recognition")
	static TArray<FVector2D> ProjectExtremeVerticesToScreen(APlayerController* LocalPlayerController, TArray<FVector> const& Vertices);

private:
	static void GetLocalVerticesWithCpuSkinning(TArray<FVector3f>& OutVertices, const USkeletalMeshComponent* MeshComponent, int32 LODIndex);

	static void GetLocalVerticesWithGpuSkinning(TArray<FVector3f>& OutVertices, USkeletalMeshComponent* MeshComponent, int32 LODIndex);

	static void TransformLocalToWorld(TArray<FVector3f>& InOutVertices, const USkeletalMeshComponent* MeshComponent);
};
