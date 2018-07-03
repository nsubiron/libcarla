// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <memory>

class AServer;

class FRPCServer
{
public:

  FRPCServer();

  ~FRPCServer();

  void Initialize(AServer &Server, uint16_t Port = 8080u);

  void Run();

  void RunSome();

  void Stop();

private:

  class Pimpl;
  std::unique_ptr<Pimpl> _Pimpl;
};
