import logging
from logging.handlers import RotatingFileHandler

class logger:
    #Setting up logger
    formatter = logging.Formatter('%(msecs)d') #Log format
    max_kbytes=2*1024

    def __init__(self):
            path = 'logs'
            self.logger_info = logging.getLogger(__name__ + '_INFO') #logger for general debugging purposes, from debug to warning levels
            self.logger_info.setLevel(logging.DEBUG)
            file_handler_info =  RotatingFileHandler(path + '/SOLIDAR_INFO.csv', mode='a', maxBytes=self.max_kbytes*1024, backupCount=2, encoding=None, delay=0)
            file_handler_info.setFormatter(self.formatter)
            self.logger_info.addHandler(file_handler_info)
            self.logger_info.info('Date,Filter,Position,Lidar,Sonar')

    def info(self,log):
        return self.logger_info.info(log)
