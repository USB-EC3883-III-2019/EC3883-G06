import numpy as np


def process_fusion(sonar,lidar):

    #Fusion
    sonvarn2 = 0.356860852
    lidvarn2 = 1.717397544
    sonarMaxRange = 300
    fusvar2 = ((sonvarn2) + (lidvarn2))**(-1)


    fusion = fusvar2*(sonvarn2*sonar + lidvarn2*lidar)
    fusion[lidar >= 80] = sonar[lidar >= 80]

    return fusion
