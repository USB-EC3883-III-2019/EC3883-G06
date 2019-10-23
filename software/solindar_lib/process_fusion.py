import numpy as np


def process_fusion(sonar,lidar):

    #Fusion
    sonvarn2 = (1.47*(10**-3))**-2
    lidvarn2 = (4.89*(10**-5))**-2
    fusvar2 = ((sonvarn2) + (lidvarn2))**(-1)

    fusion = fusvar2*(sonvarn2*sonar + lidvarn2*lidar)

    return fusion