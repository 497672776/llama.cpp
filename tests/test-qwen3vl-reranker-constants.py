import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "gguf-py"))

import gguf  # noqa: E402


def test_qwen3vl_allows_reranker_classifier_output_tensor():
    assert gguf.MODEL_TENSOR.CLS_OUT in gguf.MODEL_TENSORS[gguf.MODEL_ARCH.QWEN3VL]

