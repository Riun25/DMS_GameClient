#pragma once
#define NOMINMAX

#pragma warning(push)
#pragma warning(disable : 4635)
#pragma warning(disable : 33010)
#pragma warning(disable : 26495)
#pragma warning(disable : 5260)
#pragma warning(disable : 6297)
#pragma warning(disable : 4006)
#include <physx/PxPhysicsAPI.h>
#include <physx/pxsimulationeventcallback.h>
#include <physx/characterkinematic/PxControllerManager.h>

#ifdef _DEBUG
#include <physx/pvd/PxPvd.h>
#include <physx/pvd/PxPvdTransport.h>
#endif

#pragma warning(pop)
