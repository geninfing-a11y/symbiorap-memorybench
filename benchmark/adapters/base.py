from abc import ABC, abstractmethod
class Adapter(ABC):
    contract_version="1.0.0"
    @abstractmethod
    def put(self,key,value): ...
    @abstractmethod
    def get(self,key): ...
    @abstractmethod
    def delete(self,key): ...
    def close(self): pass
