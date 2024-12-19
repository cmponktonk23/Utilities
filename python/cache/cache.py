# cache.py
from abc import ABC, abstractmethod
from threading import RLock
from typing import Generic, TypeVar, Optional

from cache_exception import CacheException
from replacement_policy import IReplacementPolicy
from replacement_policy_factory import ReplacementPolicyFactory, ReplacementPolicyEnum

KeyType = TypeVar("KeyType")
ValueType = TypeVar("ValueType")


class IKVCache(ABC, Generic[KeyType, ValueType]):
    """Interface for key-value cache."""

    @abstractmethod
    def put(self, key: KeyType, value: ValueType) -> bool:
        pass

    @abstractmethod
    def get(self, key: KeyType) -> Optional[ValueType]:
        pass

    @abstractmethod
    def remove(self, key: KeyType) -> bool:
        pass


class CacheSystem(IKVCache[KeyType, ValueType], Generic[KeyType, ValueType]):
    """Cache system implementing the key-value cache."""

    def __init__(
        self,
        capacity: int,
        policy_type: str,
        replacement_policy_factory: Optional[ReplacementPolicyFactory[KeyType]] = None,
    ):
        self._capacity = capacity
        self._cache = {}
        self._lock = RLock()
        factory = replacement_policy_factory or ReplacementPolicyFactory()
        self._replacement_policy = factory.create(policy_type)

    def put(self, key: KeyType, value: ValueType) -> bool:
        with self._lock:
            if key in self._cache:
                self._cache[key] = value
                if not self._replacement_policy.access(key):
                    raise CacheException("Failed to update key in replacement policy.")
            else:
                if len(self._cache) >= self._capacity:
                    victim_key = self._replacement_policy.evict()
                    if victim_key is None:
                        raise CacheException(
                            "Cache is full and no key could be evicted."
                        )
                    del self._cache[victim_key]
                if not self._replacement_policy.add(key):
                    raise CacheException("Failed to add key to replacement policy.")
                self._cache[key] = value
            return True

    def get(self, key: KeyType) -> Optional[ValueType]:
        with self._lock:
            if key not in self._cache:
                return None
            if not self._replacement_policy.access(key):
                raise CacheException("Failed to access key in replacement policy.")
            return self._cache[key]

    def remove(self, key: KeyType) -> bool:
        with self._lock:
            if key not in self._cache:
                return False
            if not self._replacement_policy.remove(key):
                raise CacheException("Failed to remove key from replacement policy.")
            del self._cache[key]
            return True
