#include "DSRLGameModeBase.h"
#include "MyCharacter.h"

ADSRLGameModeBase::ADSRLGameModeBase()
{
	DefaultPawnClass = AMyCharacter::StaticClass();
}