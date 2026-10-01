from .base import Adapter
class ReferenceAdapter(Adapter):
    name="reference-memory"
    durability_class="process"
    def __init__(self, **_): self.db={}
    def put(self,k,v): self.db[k]=v; return True
    def get(self,k): return self.db.get(k)
    def delete(self,k): return self.db.pop(k,None) is not None
