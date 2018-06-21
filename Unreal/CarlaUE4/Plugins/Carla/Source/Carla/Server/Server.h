// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Vehicle/CarlaWheeledVehicle.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPCServer.h"
#include "Vehicle/VehicleControl.h"
#include <future>
#include <mutex>
#include <queue>
#include "Server.generated.h"

UCLASS()
class CARLA_API AServer : public AActor
{
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  AServer();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  std::future<int32_t> SpawnAgentAsync(const FTransform &Transform);

  UFUNCTION(BlueprintCallable)
  int32 SpawnAgent(const FTransform &Transform);

  UFUNCTION(BlueprintCallable)
  bool ApplyControl(int32 AgentId, const FVehicleControl &Control);

  UFUNCTION(BlueprintImplementableEvent)
  void SpawnVehicle(const FTransform &SpawnTransform, ACarlaWheeledVehicle *&SpawnedCharacter);

private:

  FRPCServer _Server;

  UPROPERTY(VisibleAnywhere)
  TMap<uint32, ACarlaWheeledVehicle *> _Agents;

  mutable std::mutex _Mutex;

  using IdPromise = std::promise<int32_t>;
  using Task = std::pair<FTransform, std::shared_ptr<IdPromise>>;
  std::queue<Task> _SpawnQueue;
};
