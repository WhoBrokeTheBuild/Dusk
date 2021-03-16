import Dusk

from enum import IntEnum

class LogLevel(IntEnum):
    Verbose = 0
    Performance = 1
    Debug = 2
    Info = 3
    Warning = 4
    Error = 5
    Fatal = 6
    Silent = 7

Dusk.LogLevel = LogLevel
