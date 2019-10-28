import numpy as np


def process_fusion(sonar,lidar):

    #Fusion
    sonvarn2 = 0.356860852
    lidvarn2 = 1.717397544
    sonarMaxRange = 300
    fusvar2 = ((sonvarn2)**(-1) + (lidvarn2)**(-1))**(-1)


    fusion = fusvar2*((sonvarn2**-1)*sonar + (lidvarn2**-1)*lidar)
    fusion[lidar >= 80] = sonar[lidar >= 80]

    return fusion
