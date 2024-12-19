# lru_replacement_policy.py
from collections import OrderedDict
from threading import RLock
from typing import Generic, TypeVar, Optional

from replacement_policy import IReplacementPolicy

KeyType = TypeVar("KeyType")


class LRUReplacementPolicy(IReplacementPolicy[KeyType], Generic[KeyType]):
    """Least Recently Used (LRU) cache replacement policy."""

    def __init__(self):
        self._lock = RLock()
        self._lru_order = OrderedDict()

    def add(self, key: KeyType) -> bool:
        with self._lock:
            if key in self._lru_order:
                return False
            self._lru_order[key] = None
            return True

    def remove(self, key: KeyType) -> bool:
        with self._lock:
            if key not in self._lru_order:
                return False
            del self._lru_order[key]
            return True

    def access(self, key: KeyType) -> bool:
        with self._lock:
            if key not in self._lru_order:
                return False
            self._lru_order.move_to_end(key)
            return True

    def evict(self) -> Optional[KeyType]:
        with self._lock:
            if not self._lru_order:
                return None
            key, _ = next(iter(self._lru_order.items()))
            del self._lru_order[key]
            return key
