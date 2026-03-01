import os
import torch
from transformers import (
    AutoModelForCausalLM, AutoTokenizer,
    TrainingArguments, DataCollatorForLanguageModeling,
    Trainer, BitsAndBytesConfig
)
from datasets import load_dataset
from peft import LoraConfig, get_peft_model
if __name__ == "__main__":
    os.environ["PYTORCH_CUDA_ALLOC_CONF"] = "max_split_size_mb:32,garbage_collection_threshold:0.6"
    model_path = "./model"
    version = 2

    tokenizer = AutoTokenizer.from_pretrained(model_path, trust_remote_code=True)
    tokenizer.pad_token = tokenizer.eos_token

    bnb_config = BitsAndBytesConfig(
        load_in_4bit=True,
        bnb_4bit_quant_type="nf4",
        bnb_4bit_use_double_quant=True,
    )
    model = AutoModelForCausalLM.from_pretrained(
        model_path,
        quantization_config=bnb_config,
        device_map="auto",
        trust_remote_code=True,
    )
    lora_config = LoraConfig(
        r=16,
        lora_alpha=32,
        target_modules=["q_proj", "v_proj"],
        lora_dropout=0.05,
        bias="none",
        task_type="CAUSAL_LM"
    )
    model = get_peft_model(model, lora_config)
    model.enable_input_require_grads()
    dataset = load_dataset('json', data_files={
        'train': 'rag_train_set.jsonl',
        'validation': 'rag_test_set.jsonl'
    })
    def format_instruction(sample):
        user_msg, assistant_msg = "", ""
        for msg in sample["messages"]:
            if msg["role"] == "user":
                user_msg += msg["content"].strip() + "\n"
            elif msg["role"] == "assistant":
                assistant_msg += msg["content"].strip() + "\n"
        return f"### Instruction:\n{user_msg}\n### Response:\n{assistant_msg}"

    def tokenize_function(examples):
        texts = [format_instruction({"messages": msgs}) for msgs in examples["messages"]]
        return tokenizer(texts, truncation=True, max_length=512, padding="max_length")

    tokenized_dataset = dataset.map(
        tokenize_function,
        batched=True,
        remove_columns=dataset["train"].column_names
    )

    data_collator = DataCollatorForLanguageModeling(tokenizer=tokenizer, mlm=False)
    training_args = TrainingArguments(
        output_dir=f"./deepseek-coder-v{version}",
        num_train_epochs=3,
        per_device_train_batch_size=1,
        gradient_accumulation_steps=16,
        learning_rate=2e-4,
        fp16=True,
        logging_steps=50,
        save_strategy="epoch",
        remove_unused_columns=True,
        dataloader_num_workers=1,
        optim="paged_adamw_8bit"
    )
    trainer = Trainer(
        model=model,
        args=training_args,
        train_dataset=tokenized_dataset["train"],
        eval_dataset=tokenized_dataset["validation"],
        tokenizer=tokenizer,
        data_collator=data_collator
    )
    torch.cuda.empty_cache()
    print(" بدء التدريب باستخدام LoRA 4-bit...")
    trainer.train()
    trainer.save_model()
    print(" تم الانتهاء من التدريب بنجاح")
