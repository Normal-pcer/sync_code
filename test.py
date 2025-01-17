from abc import ABC, abstractmethod


class A(ABC):
    @abstractmethod
    def getTarget(self):
        pass

    def work(self):
        print("fuck", self.getTarget())

class B(A):
    def __init__(self):
        pass
    @staticmethod
    def getTarget():
        return "ccf"

x = A()
x.work()
