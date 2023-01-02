// BEGIN OF METADATA
// THIS METADATA ARE GENERATE BY EDITOR, PLEASE DO NOT REMOVE ITS
/* 
{
    "name": "main player",
    "states": [ "idle", "move", "jump", "attack" ],
    "entry": "idle",
    "transitions": [ "idle:move", "idle:attack", "idle:jump" ],
    "handlers": {
        "idle": "MainPlayer_IdleState",
        "move": "MainPlayer_MoveState"
    }
} 
*/
// END OF METADATA

struct MainPlayer_StateMachine
{
    const char* name;
    // More members...
};
