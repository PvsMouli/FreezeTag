# Session Management Decision

## Summary
We will handle session creation and joining in the GameInstance.

MainMenuWidget
↓ calls
PlayerController
↓ calls
GameInstance → SessionSubsystem
↓ creates session
↓ on success


## Reasoning
- GameInstance persists between levels  
- Easy access to online subsystem  
- Cleaner Blueprint and C++ structure 