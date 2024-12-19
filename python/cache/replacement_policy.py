# replacement_policy.py
from abc import ABC, abstractmethod
from typing import Generic, TypeVar, Optional

KeyType = TypeVar("KeyType")


class IReplacementPolicy(ABC, Generic[KeyType]):
    """Interface for cache replacement policies."""

    @abstractmethod
    def add(self, key: KeyType) -> bool:
        pass

    @abstractmethod
    def remove(self, key: KeyType) -> bool:
        pass

    @abstractmethod
    def access(self, key: KeyType) -> bool:
        pass

    @abstractmethod
    def evict(self) -> Optional[KeyType]:
        pass
