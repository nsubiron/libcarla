// Fill out your copyright notice in the Description page of Project Settings.

#include "Carla.h"
#include "Server.h"


// Sets default values
AServer::AServer()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AServer::BeginPlay()
{
  Super::BeginPlay();

  _Server.Initialize(*this);
  _Server.Run();
}

// Called every frame
void AServer::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  std::lock_guard<std::mutex> guard(_Mutex);
  if (_SpawnQueue.empty())
    return;

  auto task = _SpawnQueue.front();
  _SpawnQueue.pop();
  task.second->set_value(SpawnAgent(task.first));
}

std::future<int32_t> AServer::SpawnAgentAsync(const FTransform &Transform)
{
  std::lock_guard<std::mutex> guard(_Mutex);
  auto Promise = std::make_shared<IdPromise>();
  auto future = Promise->get_future();
  _SpawnQueue.push(std::make_pair(Transform, Promise));
  return future;
}

int32 AServer::SpawnAgent(const FTransform &Transform)
{
  check(IsInGameThread());
  UE_LOG(LogTemp, Warning, TEXT("Spawning vehicle at %s"), *Transform.ToString());

  static int32 COUNT = 0u;
  ++COUNT;

  ACarlaWheeledVehicle *Vehicle;
  SpawnVehicle(Transform, Vehicle);
  if ((Vehicle != nullptr) && !Vehicle->IsPendingKill())
  {
    // Vehicle->AIControllerClass = AWheeledVehicleAIController::StaticClass();
    Vehicle->SpawnDefaultController();
    _Agents.Add(COUNT, Vehicle);
    return COUNT;
  }
  return -1;
}

bool AServer::ApplyControl(int32 AgentId, const FVehicleControl &Control)
{
  UE_LOG(LogTemp, Log, TEXT("Applying control to vehicle %d: throttle = %f, steer = %f"), AgentId, Control.Throttle, Control.Steer);
  if (!_Agents.Contains(AgentId))
  {
    UE_LOG(LogTemp, Error, TEXT("Vehicle %d does not exist!"), AgentId);
    return false;
  }
  auto *Vehicle = _Agents[AgentId];
  Vehicle->ApplyVehicleControl(Control);
  return true;
}
