import numpy as np


def process_fusion(sonar,lidar):

    #Fusion
    sonvarn2 = 0.489256966
    lidvarn2 = 0.0021*(lidar**2)-0.0958*lidar+0.9795
    sonarMaxRange = 300
    fusvar2 = ((sonvarn2)**(-1) + (lidvarn2)**(-1))**(-1)


    fusion = fusvar2*((sonvarn2**-1)*sonar + (lidvarn2**-1)*lidar)
    fusion[lidar >= 80] = sonar[lidar >= 80]

    return fusion
