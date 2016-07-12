#    File:    Singleton.py
#    Author:  Marvin Smith
#    Date:   7/12/2016
#
#    Purpose: Provide Singleton Background for Objects to Copy
#

class Singleton(type):

    #  Define the set of instances
    _instances = {}
    def __call__(cls, *args, **kwargs):

        if cls not in cls._instances:
            cls._instances[cls] = super(Singleton, cls).__call__(*args, **kwargs)
        return cls._instances[cls]
