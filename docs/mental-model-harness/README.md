# Spheres.io Mental-Model Harness

A portable, adaptive pedagogical harness and knowledge repository for **CO1035 / CO1301 Games Concepts** (UCLan Cyprus).

Inspired by `learnharness`, this repository isolates the **teaching protocol, world knowledge, curriculum contracts, and student mastery model** so that any AI assistant (Antigravity, Claude, ChatGPT, Gemini, Cursor) can act as an effective Socratic pair-programming tutor.

---

## Repository Structure

```
SpheresIO-MentalModel-Harness/
├── README.md                              <- Overview and LLM transfer guide
├── SYSTEM_PROMPT_AGENT_BEHAVIOR.md        <- Behavioral rules, visual diagram mandate, no-fluff rule
├── WORLD_MODEL_CURRICULUM.md              <- 3D coordinate rules, vector math, collision equations
├── SPHERES_IO_MILESTONE_SPECIFICATIONS.md <- Rubric breakdown & acceptance tests (Milestones 1–5)
├── LEARNER_EVIDENCE_MODEL.md              <- Student mastery profile, resolved misconceptions, active frontier
├── sync_to_gdrive.bat                     <- One-click sync script to Google Drive
└── .gitignore                             <- Git ignore rules
```

---

## How to Use This with Any AI / LLM

When starting a new session on your laptop or with another AI assistant:

1. **Copy the contents of `SYSTEM_PROMPT_AGENT_BEHAVIOR.md`** into the AI's custom instructions or system prompt.
2. Provide `WORLD_MODEL_CURRICULUM.md` and `SPHERES_IO_MILESTONE_SPECIFICATIONS.md` as context documents or attach them to the chat.
3. The AI will immediately adopt the correct tutor persona:
   - Provide visual diagrams for all spatial/coordinate concepts.
   - Keep answers concise and direct without conversational fluff.
   - Never write the complete assignment code; guide you to write it yourself.
   - Strictly adhere to the single `.cpp` file university rubric.

---

## Synchronization Options

### 1. Google Drive Sync
Run `sync_to_gdrive.bat` (or `sync_to_gdrive.ps1`) to automatically mirror this repository into your Google Drive folder:
```
G:\My Drive\UCLan\SpheresIO-MentalModel-Harness\
```

### 2. GitHub Private Repository
To push this harness to GitHub as a private repository:
```powershell
gh auth login
gh repo create SpheresIO-MentalModel-Harness --private --source="." --remote=origin --push
```
