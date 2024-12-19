# lfu_replacement_policy.py
from collections import defaultdict
from threading import RLock
from typing import Generic, TypeVar, Optional

from replacement_policy import IReplacementPolicy

KeyType = TypeVar("KeyType")


class LFUReplacementPolicy(IReplacementPolicy[KeyType], Generic[KeyType]):
    """Least Frequently Used (LFU) cache replacement policy."""

    def __init__(self):
        self._lock = RLock()
        self._key_freq = defaultdict(int)
        self._freq_keys = defaultdict(set)
        self._min_freq = 0

    def add(self, key: KeyType) -> bool:
        with self._lock:
            if key in self._key_freq:
                return False
            self._key_freq[key] = 1
            self._freq_keys[1].add(key)
            self._min_freq = 1
            return True

    def remove(self, key: KeyType) -> bool:
        with self._lock:
            if key not in self._key_freq:
                return False
            freq = self._key_freq.pop(key)
            self._freq_keys[freq].remove(key)
            if not self._freq_keys[freq]:
                del self._freq_keys[freq]
                if freq == self._min_freq:
                    self._min_freq += 1
            return True

    def access(self, key: KeyType) -> bool:
        with self._lock:
            if key not in self._key_freq:
                return False
            freq = self._key_freq[key]
            self._key_freq[key] += 1
            self._freq_keys[freq].remove(key)
            if not self._freq_keys[freq]:
                del self._freq_keys[freq]
                if freq == self._min_freq:
                    self._min_freq += 1
            self._freq_keys[freq + 1].add(key)
            return True

    def evict(self) -> Optional[KeyType]:
        with self._lock:
            if not self._key_freq:
                return None
            key = next(iter(self._freq_keys[self._min_freq]))
            self.remove(key)
            return key
