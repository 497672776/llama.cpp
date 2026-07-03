from pathlib import Path
import unittest


ROOT = Path(__file__).resolve().parents[1]


class Qwen3VLRerankerTemplateTest(unittest.TestCase):
    def test_qwen3vl_rerank_server_uses_original_reranker_template(self):
        server_common = (ROOT / "tools/server/server-common.cpp").read_text()
        server_context = (ROOT / "tools/server/server-context.cpp").read_text()

        self.assertIn('llama_model_chat_template(model, "reranker")', server_common)
        self.assertIn('llama_model_chat_template(ctx_server.model_tgt, "reranker")', server_context)

    def test_multimodal_rerank_inserts_media_markers_inside_query_and_document(self):
        server_common_h = (ROOT / "tools/server/server-common.h").read_text()
        server_common = (ROOT / "tools/server/server-common.cpp").read_text()
        server_context = (ROOT / "tools/server/server-context.cpp").read_text()

        self.assertIn("n_query_media", server_common_h)
        self.assertIn("n_doc_media", server_common_h)
        self.assertIn("query.multimodal_data.size()", server_context)
        self.assertIn("document.multimodal_data.size()", server_context)
        self.assertIn("append_media_markers(prompt, media_markers_to_insert(query, n_query_media))", server_common)
        self.assertIn("append_media_markers(prompt, media_markers_to_insert(doc, n_doc_media))", server_common)

    def test_smt_qwen3vl_image_embeddings_are_expanded_to_model_input_width(self):
        server_smt_vision = (ROOT / "tools/server/server-smt-vision.cpp").read_text()

        self.assertIn("llama_model_n_embd_inp", server_smt_vision)
        self.assertIn("expand_deepstack_image_embeddings", server_smt_vision)
        self.assertIn("n_model_embd_inp", server_smt_vision)
        self.assertIn("chunk.embd.size() / (size_t) n_embd_tokens", server_smt_vision)

    def test_qwen3vl_embedding_endpoint_applies_model_chat_template(self):
        server_context = (ROOT / "tools/server/server-context.cpp").read_text()

        self.assertIn("should_apply_qwen3vl_embedding_template", server_context)
        self.assertIn("format_qwen3vl_embedding_prompt", server_context)
        self.assertIn("format_qwen3vl_embedding_inputs", server_context)
        self.assertIn("prompt = format_qwen3vl_embedding_inputs", server_context)

    def test_prompt_object_multimodal_data_accepts_data_urls(self):
        server_common = (ROOT / "tools/server/server-common.cpp").read_text()

        self.assertIn("base64_decode_multimodal_data", server_common)
        self.assertIn('string_starts_with(parts[0], "data:")', server_common)
        self.assertIn('string_ends_with(parts[0], "base64")', server_common)
        self.assertIn("files.push_back(base64_decode_multimodal_data(entry))", server_common)


if __name__ == "__main__":
    unittest.main()
