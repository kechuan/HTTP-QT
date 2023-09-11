#ifndef ENUMINDEX_H
#define ENUMINDEX_H

//Server FileList
enum SurfingFileColumn{
    iconList,
    nameList,
    sizeList,
    linkList
};

enum ResourceType{
    Dir,
    Text,
    Image,
    Video,
    Music,
    Compress,
    Code,
    UnknownFile
};


//propList
enum QueueList{
    FilenameList,
    ProgressList,
    SizeList,
    SpeedList,
    DateTimeList,
    StoragePathList,
};

enum StatusList{
    Pending,
    Downloading,
    Uploading,
    Paused,
    Finished,
    Failed,
};

//connect Part

enum AccessType{
    Guest,
    User,
    Admin
};

enum TaskStatus{
    TaskCompleted,
    TaskPaused,
    TaskCanceled
};

enum SpeedLimitRank{
    LowLimit,
    MediumLimit,
    HighLimit
};

#endif

