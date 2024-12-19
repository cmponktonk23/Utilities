# cache_exception.py
from typing import Any


class CacheException(Exception):
    """Custom exception for cache errors."""

    def __init__(self, message: str):
        super().__init__(message)