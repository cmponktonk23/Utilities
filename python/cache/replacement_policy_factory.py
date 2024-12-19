# replacement_policy_factory.py
from abc import ABC, abstractmethod
from typing import Generic, TypeVar

from replacement_policy import IReplacementPolicy
from lru_replacement_policy import LRUReplacementPolicy
from lfu_replacement_policy import LFUReplacementPolicy
from cache_exception import CacheException

KeyType = TypeVar("KeyType")


class ReplacementPolicyEnum:
    """Enumeration of replacement policies."""

    LRU = "LRU"
    LFU = "LFU"


class IReplacementPolicyFactory(ABC, Generic[KeyType]):
    """Interface for replacement policy factory."""

    @abstractmethod
    def create(self, policy_type: str) -> IReplacementPolicy[KeyType]:
        pass


class ReplacementPolicyFactory(IReplacementPolicyFactory[KeyType], Generic[KeyType]):
    """Factory to create replacement policy instances."""

    def create(self, policy_type: str) -> IReplacementPolicy[KeyType]:
        if policy_type == ReplacementPolicyEnum.LRU:
            return LRUReplacementPolicy()
        elif policy_type == ReplacementPolicyEnum.LFU:
            return LFUReplacementPolicy()
        else:
            raise CacheException(f"Unsupported replacement policy: {policy_type}")
