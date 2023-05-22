/*
 * audio_player.h
 *
 * Created by Ruibin.Chow on 2023/05/22.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

class AudioPlayer {
    
public:
    virtual ~AudioPlayer() = default;
    
    void Init();
};


#endif /* !AUDIO_PLAYER_H */
