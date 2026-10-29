#!/usr/bin/env python3
"""
build_world_model.py
Generates world_model/propositions.jsonl and world_model/reasoning_chains.jsonl.
Strict adherence to module sources and citation rules.
"""

import json
import os

PROPOSITIONS = [
    # ---------------------------------------------------------
    # DOMAIN: assessment
    # ---------------------------------------------------------
    {
        "id": "P-ASSESSMENT-001",
        "statement": "The module code is CO1301 (also referenced as CO1035 in teaching materials), delivered across Semesters 1 and 2 for 10 ECTS / 20 UK credits.",
        "domain": "assessment",
        "week": 1,
        "prerequisites": [],
        "source_refs": [
            {"file": "sources/uclancyprus-module-descriptor-CO1301.txt", "loc": "line 4"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Lectures___CO1035 - Lecture 1A - Introduction to the Module.pptx.txt", "loc": "slide 1"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-ASSESSMENT-002",
        "statement": "The module content is organized into four related threads: game mechanics, commercial processes, basic game development, and mathematics.",
        "domain": "assessment",
        "week": 1,
        "prerequisites": ["P-ASSESSMENT-001"],
        "source_refs": [
            {"file": "sources/uclancyprus-module-descriptor-CO1301.txt", "loc": "line 10"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Lectures___CO1035 - Lecture 1A - Introduction to the Module.pptx.txt", "loc": "slide 3"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-ASSESSMENT-003",
        "statement": "The module has four learning outcomes: describe key game concepts, critique a game design, use a game engine to create simple prototypes, and apply mathematical techniques for analysis.",
        "domain": "assessment",
        "week": 1,
        "prerequisites": ["P-ASSESSMENT-001"],
        "source_refs": [
            {"file": "sources/uclancyprus-module-descriptor-CO1301.txt", "loc": "line 16"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Lectures___CO1035 - Lecture 1A - Introduction to the Module.pptx.txt", "loc": "slide 4"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-ASSESSMENT-004",
        "statement": "Lecture 1A slide 13 specifies the module assessment as two programming assignments: Assignment 1 weighted at 40% and Assignment 2 weighted at 60%.",
        "domain": "assessment",
        "week": 1,
        "prerequisites": ["P-ASSESSMENT-001"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Lectures___CO1035 - Lecture 1A - Introduction to the Module.pptx.txt", "loc": "slide 13"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-ASSESSMENT-005",
        "statement": "The formal module descriptor states that assessment is conducted through a portfolio of games and an in-class maths test, presenting an unresolved discrepancy with Lecture 1A.",
        "domain": "assessment",
        "week": 1,
        "prerequisites": ["P-ASSESSMENT-001", "P-ASSESSMENT-004"],
        "source_refs": [
            {"file": "sources/uclancyprus-module-descriptor-CO1301.txt", "loc": "line 22"}
        ],
        "kind": "warning"
    },
    {
        "id": "P-ASSESSMENT-006",
        "statement": "The Blackboard Assessment 1 page explicitly lists 5 attached documents and a video: Assessment Brief, Assignment Resources, Guidance Document, Report Template, Coursework Cover Sheet, and Video Demonstration.",
        "domain": "assessment",
        "week": None,
        "prerequisites": ["P-ASSESSMENT-004"],
        "source_refs": [
            {"file": "sources/blackboard_2026-27/Assessments/Assessment 1/CO1035 - Assignment 1 Brief/ultraDocumentBody - page.txt", "loc": "lines 3-10"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-ASSESSMENT-007",
        "statement": "The five Assignment 1 brief documents and video demonstration are missing from the course sources, forming a known gap where requirements, grade bands, and deadlines cannot be invented.",
        "domain": "assessment",
        "week": None,
        "prerequisites": ["P-ASSESSMENT-006"],
        "source_refs": [
            {"file": "sources/blackboard_2026-27/Assessments/Assessment 1/CO1035 - Assignment 1 Brief/ultraDocumentBody - page.txt", "loc": "lines 3-18"}
        ],
        "kind": "warning"
    },
    {
        "id": "P-ASSESSMENT-008",
        "statement": "Institutional Module Evaluation Questionnaire is conducted online via Microsoft Forms during the final weeks of teaching to capture anonymous feedback.",
        "domain": "assessment",
        "week": None,
        "prerequisites": ["P-ASSESSMENT-001"],
        "source_refs": [
            {"file": "sources/blackboard_2026-27/Institutional Module Evaluation Questionnaire/Institutional Module Evaluation Questionnaire/ultraDocumentBody - page.txt", "loc": "lines 3-9"}
        ],
        "kind": "fact"
    },

    # ---------------------------------------------------------
    # DOMAIN: setup
    # ---------------------------------------------------------
    {
        "id": "P-SETUP-001",
        "statement": "Visual Studio (Community 2022 or higher) is required for TL-Engine development on Windows 10 (Creators Update+) or Windows 11.",
        "domain": "setup",
        "week": 1,
        "prerequisites": [],
        "source_refs": [
            {"file": "sources/text/Setting-up my Development Environemnt___Setup Programming Environment.pdf.txt", "loc": "page 1"},
            {"file": "sources/TL-Engine11 ReadMe.txt", "loc": "lines 29-30"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-SETUP-002",
        "statement": "In the Visual Studio Installer, the user must select both 'Desktop development with C++' and 'Game Development with C++' workloads before installing.",
        "domain": "setup",
        "week": 1,
        "prerequisites": ["P-SETUP-001"],
        "source_refs": [
            {"file": "sources/text/Setting-up my Development Environemnt___Setup Programming Environment.pdf.txt", "loc": "page 1"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-SETUP-003",
        "statement": "The TL-Engine installer (e.g. TL-Engine11 1.0 Alpha7.zip) must be downloaded from Blackboard and installed to default system paths.",
        "domain": "setup",
        "week": 1,
        "prerequisites": ["P-SETUP-001"],
        "source_refs": [
            {"file": "sources/text/Setting-up my Development Environemnt___Setup Programming Environment.pdf.txt", "loc": "page 3"},
            {"file": "sources/TL-Engine11 ReadMe.txt", "loc": "line 11"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-SETUP-004",
        "statement": "The computer must be restarted after installing Visual Studio and again after installing the TL-Engine so system configuration changes can take effect.",
        "domain": "setup",
        "week": 1,
        "prerequisites": ["P-SETUP-002", "P-SETUP-003"],
        "source_refs": [
            {"file": "sources/text/Setting-up my Development Environemnt___Setup Programming Environment.pdf.txt", "loc": "pages 2-3"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-SETUP-005",
        "statement": "New projects are created via the Start menu shortcut 'Create New TL11 Project...' or by running 'TemplateWizard.exe' inside the TL-Engine11 installation directory.",
        "domain": "setup",
        "week": 1,
        "prerequisites": ["P-SETUP-003"],
        "source_refs": [
            {"file": "sources/text/Setting-up my Development Environemnt___Setup Programming Environment.pdf.txt", "loc": "page 3"},
            {"file": "sources/TL-Engine11 ReadMe.txt", "loc": "lines 13-17"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-SETUP-006",
        "statement": "In the Template Wizard window, the user selects Visual Studio 2022, names the project, selects a file directory, and clicks OK to generate the solution.",
        "domain": "setup",
        "week": 1,
        "prerequisites": ["P-SETUP-005"],
        "source_refs": [
            {"file": "sources/text/Setting-up my Development Environemnt___Setup Programming Environment.pdf.txt", "loc": "page 3"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-SETUP-007",
        "statement": "If Visual Studio reports missing dependencies when first opening a generated project, the user must click 'Install all' in the top right, click 'Apply', and restart the PC when completed.",
        "domain": "setup",
        "week": 1,
        "prerequisites": ["P-SETUP-006"],
        "source_refs": [
            {"file": "sources/text/Setting-up my Development Environemnt___Setup Programming Environment.pdf.txt", "loc": "pages 3-5"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-SETUP-008",
        "statement": "Pressing 'Local Windows Debugger' compiles and executes the C++ code, launching both a console window and an empty DirectX graphics window.",
        "domain": "setup",
        "week": 1,
        "prerequisites": ["P-SETUP-006"],
        "source_refs": [
            {"file": "sources/text/Setting-up my Development Environemnt___Setup Programming Environment.pdf.txt", "loc": "page 6"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-SETUP-009",
        "statement": "Mac users cannot run TL-Engine natively and must use a Windows Virtual Machine to run Visual Studio and the engine.",
        "domain": "setup",
        "week": 1,
        "prerequisites": ["P-SETUP-001"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Lectures___CO1035 - Lecture 1A - Introduction to the Module.pptx.txt", "loc": "slide 10"}
        ],
        "kind": "warning"
    },

    # ---------------------------------------------------------
    # DOMAIN: cpp_basics
    # ---------------------------------------------------------
    {
        "id": "P-CPP_BASICS-001",
        "statement": "The main() function is the mandatory entry point where program execution begins in every C++ executable, returning int 0 upon successful termination.",
        "domain": "cpp_basics",
        "week": 1,
        "prerequisites": [],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1A - Introduction to C .pdf.txt", "loc": "page 5"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-CPP_BASICS-002",
        "statement": "The <iostream> library provides standard stream objects: std::cout writes console output and std::cin reads keyboard input, simplified with 'using namespace std;'.",
        "domain": "cpp_basics",
        "week": 1,
        "prerequisites": ["P-CPP_BASICS-001"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1A - Introduction to C .pdf.txt", "loc": "page 6"}
        ],
        "kind": "api"
    },
    {
        "id": "P-CPP_BASICS-003",
        "statement": "Fundamental C++ data types include int for whole numbers, float and double for decimal values, bool for true/false flags, char for single characters, and std::string for text.",
        "domain": "cpp_basics",
        "week": 1,
        "prerequisites": ["P-CPP_BASICS-001"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1A - Introduction to C .pdf.txt", "loc": "page 6"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1B - Introduction to TL Engine and Game Programing.pdf.txt", "loc": "page 1"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-CPP_BASICS-004",
        "statement": "The single equals sign (=) assigns a value to a variable, whereas the double equals sign (==) tests for equality between expressions.",
        "domain": "cpp_basics",
        "week": 1,
        "prerequisites": ["P-CPP_BASICS-003"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1A - Introduction to C .pdf.txt", "loc": "page 7"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1B - Introduction to TL Engine and Game Programing.pdf.txt", "loc": "page 2"}
        ],
        "kind": "warning"
    },
    {
        "id": "P-CPP_BASICS-005",
        "statement": "Integer division truncates fractional results; at least one operand must be a floating-point literal (e.g. 5.0 / 9.0) to perform floating-point division.",
        "domain": "cpp_basics",
        "week": 1,
        "prerequisites": ["P-CPP_BASICS-003"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1A - Introduction to C .pdf.txt", "loc": "page 6"}
        ],
        "kind": "warning"
    },
    {
        "id": "P-CPP_BASICS-006",
        "statement": "An if / else if / else conditional chain evaluates conditions sequentially and executes only the first branch whose condition evaluates to true.",
        "domain": "cpp_basics",
        "week": 1,
        "prerequisites": ["P-CPP_BASICS-003", "P-CPP_BASICS-004"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1A - Introduction to C .pdf.txt", "loc": "pages 7-8"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-CPP_BASICS-007",
        "statement": "A switch statement compares an integral or enum expression against case labels, requiring break statements to prevent unintended fall-through into subsequent cases.",
        "domain": "cpp_basics",
        "week": 1,
        "prerequisites": ["P-CPP_BASICS-006"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1A - Introduction to C .pdf.txt", "loc": "pages 8-9"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-CPP_BASICS-008",
        "statement": "Logical operators combine conditions: && (AND) requires both operands true, || (OR) requires at least one true, and ! (NOT) inverts a condition, using short-circuit evaluation.",
        "domain": "cpp_basics",
        "week": 1,
        "prerequisites": ["P-CPP_BASICS-006"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1A - Introduction to C .pdf.txt", "loc": "pages 9-10"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-CPP_BASICS-009",
        "statement": "A function encapsulates reusable code with a return type, name, formal parameters, and body; non-void functions return values using the return keyword.",
        "domain": "cpp_basics",
        "week": 1,
        "prerequisites": ["P-CPP_BASICS-001", "P-CPP_BASICS-003"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1A - Introduction to C .pdf.txt", "loc": "page 16"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-CPP_BASICS-010",
        "statement": "A struct is a user-defined composite type that groups heterogeneous data members under one identifier, whose members are accessed via the dot operator (.).",
        "domain": "cpp_basics",
        "week": 1,
        "prerequisites": ["P-CPP_BASICS-003"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1A - Introduction to C .pdf.txt", "loc": "pages 17-18"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-CPP_BASICS-011",
        "statement": "An enum defines a custom type whose values are restricted to an explicit set of named integral constants, improving readability in state tracking.",
        "domain": "cpp_basics",
        "week": 3,
        "prerequisites": ["P-CPP_BASICS-003"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Lectures___CO1035 - Lecture 3A - Finite State Machines.pptx.txt", "loc": "slide 23"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-CPP_BASICS-012",
        "statement": "Variables declared with the const qualifier cannot be modified after initialization, and the compiler prevents any subsequent assignment.",
        "domain": "cpp_basics",
        "week": 2,
        "prerequisites": ["P-CPP_BASICS-003"],
        "source_refs": [
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Labs___Lab 2A - Texture Manipulation - Mouse Control - Custon Camera Control.pdf.txt", "loc": "pages 3-4"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-CPP_BASICS-013",
        "statement": "A pointer variable holds a memory address, declared using an asterisk (*); methods on objects referenced by pointers are invoked using the arrow operator (->).",
        "domain": "cpp_basics",
        "week": 1,
        "prerequisites": ["P-CPP_BASICS-003"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1B - Introduction to TL Engine and Game Programing.pdf.txt", "loc": "pages 2-3"}
        ],
        "kind": "definition"
    },

    # ---------------------------------------------------------
    # DOMAIN: engine_loop
    # ---------------------------------------------------------
    {
        "id": "P-ENGINE_LOOP-001",
        "statement": "Every TL-Engine program includes the main header <TL-Engine.h> (or <TL-Engine11.h>) and opens the engine namespace with 'using namespace tle;'.",
        "domain": "engine_loop",
        "week": 1,
        "prerequisites": ["P-CPP_BASICS-001"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1B - Introduction to TL Engine and Game Programing.pdf.txt", "loc": "page 3"},
            {"file": "sources/tl-engine11_headers/TL-Engine11.h", "loc": "lines 10-12"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-ENGINE_LOOP-002",
        "statement": "New3DEngine(EngineType engineType) creates and returns a pointer to TLEngine (aliased as I3DEngine in legacy code), passing TL11 or TLXStandard.",
        "domain": "engine_loop",
        "week": 1,
        "prerequisites": ["P-ENGINE_LOOP-001", "P-CPP_BASICS-013"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/TL-Engine11.h", "loc": "lines 20-30"},
            {"file": "sources/tl-engine11_headers/LegacyNames.h", "loc": "lines 12-13, 38-39"}
        ],
        "kind": "api"
    },
    {
        "id": "P-ENGINE_LOOP-003",
        "statement": "StartWindowed(int width = 1600, int height = 900) opens the 3D graphics window; StartFullscreen(width, height) opens fullscreen mode.",
        "domain": "engine_loop",
        "week": 1,
        "prerequisites": ["P-ENGINE_LOOP-002"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "lines 44-48"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1B - Introduction to TL Engine and Game Programing.pdf.txt", "loc": "page 4"}
        ],
        "kind": "api"
    },
    {
        "id": "P-ENGINE_LOOP-004",
        "statement": "Game programs adhere to the game code structure divided into three sections: initialization (setup and loading), game loop (rendering and updates), and termination (cleanup).",
        "domain": "engine_loop",
        "week": 1,
        "prerequisites": ["P-ENGINE_LOOP-001"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1B - Introduction to TL Engine and Game Programing.pdf.txt", "loc": "pages 3-4"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-ENGINE_LOOP-005",
        "statement": "The main game loop runs continuously using 'while (myEngine->IsRunning())', executing rendering and scene update logic repeatedly each frame until stopped.",
        "domain": "engine_loop",
        "week": 1,
        "prerequisites": ["P-ENGINE_LOOP-004"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "line 69"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1B - Introduction to TL Engine and Game Programing.pdf.txt", "loc": "pages 4-5"}
        ],
        "kind": "api"
    },
    {
        "id": "P-ENGINE_LOOP-006",
        "statement": "DrawScene(Camera* camera = nullptr) renders all 3D scene objects from the perspective of the specified camera (or most recently created camera if omitted).",
        "domain": "engine_loop",
        "week": 1,
        "prerequisites": ["P-ENGINE_LOOP-005"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "lines 167"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1B - Introduction to TL Engine and Game Programing.pdf.txt", "loc": "page 4"}
        ],
        "kind": "api"
    },
    {
        "id": "P-ENGINE_LOOP-007",
        "statement": "Calling myEngine->Stop() signals the engine to terminate execution, causing IsRunning() to return false and ending the main loop.",
        "domain": "engine_loop",
        "week": 1,
        "prerequisites": ["P-ENGINE_LOOP-005"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "line 59"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1C - Conditionals and Player Input.pdf.txt", "loc": "page 4"}
        ],
        "kind": "api"
    },
    {
        "id": "P-ENGINE_LOOP-008",
        "statement": "Calling myEngine->Delete() in the termination section cleans up all allocated engine memory; omitting it causes a memory leak.",
        "domain": "engine_loop",
        "week": 1,
        "prerequisites": ["P-ENGINE_LOOP-004"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "line 53"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1B - Introduction to TL Engine and Game Programing.pdf.txt", "loc": "page 5"}
        ],
        "kind": "warning"
    },
    {
        "id": "P-ENGINE_LOOP-009",
        "statement": "AddMediaFolder(const string& folderName) registers a folder path searched by the engine when loading 3D meshes, textures, and fonts.",
        "domain": "engine_loop",
        "week": 1,
        "prerequisites": ["P-ENGINE_LOOP-002"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "line 93"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1B - Introduction to TL Engine and Game Programing.pdf.txt", "loc": "page 4"}
        ],
        "kind": "api"
    },
    {
        "id": "P-ENGINE_LOOP-010",
        "statement": "Default media files reside at 'C:\\ProgramData\\TL-Engine\\Media', but projects requiring custom assets set local paths such as '.\\media'.",
        "domain": "engine_loop",
        "week": 1,
        "prerequisites": ["P-ENGINE_LOOP-009"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1B - Introduction to TL Engine and Game Programing.pdf.txt", "loc": "page 4"},
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Labs___Lab 2C - Field of Spikes (Game Prototype).pdf.txt", "loc": "page 1"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-ENGINE_LOOP-011",
        "statement": "IsActive() returns whether the engine window currently possesses OS foreground focus; input should be ignored when IsActive() returns false.",
        "domain": "engine_loop",
        "week": 1,
        "prerequisites": ["P-ENGINE_LOOP-005"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "lines 75-76"}
        ],
        "kind": "api"
    },

    # ---------------------------------------------------------
    # DOMAIN: scene_objects
    # ---------------------------------------------------------
    {
        "id": "P-SCENE_OBJECTS-001",
        "statement": "A Mesh (Mesh / IMesh) represents 3D geometric data loaded from disk via myEngine->LoadMesh(\"filename.x\"), serving as a reusable blueprint.",
        "domain": "scene_objects",
        "week": 1,
        "prerequisites": ["P-ENGINE_LOOP-009", "P-CPP_BASICS-013"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "line 111"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1B - Introduction to TL Engine and Game Programing.pdf.txt", "loc": "pages 5-6"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-SCENE_OBJECTS-002",
        "statement": "A Model (Model / IModel) is an individual instance of a mesh placed in the scene, created via mesh->CreateModel(x, y, z).",
        "domain": "scene_objects",
        "week": 1,
        "prerequisites": ["P-SCENE_OBJECTS-001"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/Mesh.h", "loc": "line 38"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1B - Introduction to TL Engine and Game Programing.pdf.txt", "loc": "pages 5-6"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-SCENE_OBJECTS-003",
        "statement": "A single loaded Mesh pointer can be used to instantiate numerous Model instances in the scene without reloading the mesh file from disk.",
        "domain": "scene_objects",
        "week": 1,
        "prerequisites": ["P-SCENE_OBJECTS-001", "P-SCENE_OBJECTS-002"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1B - Introduction to TL Engine and Game Programing.pdf.txt", "loc": "page 5, page 8"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-SCENE_OBJECTS-004",
        "statement": "If CreateModel() is called with no arguments, the model instance is positioned at the scene origin (0, 0, 0).",
        "domain": "scene_objects",
        "week": 1,
        "prerequisites": ["P-SCENE_OBJECTS-002"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/Mesh.h", "loc": "line 38"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1B - Introduction to TL Engine and Game Programing.pdf.txt", "loc": "page 8"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-SCENE_OBJECTS-005",
        "statement": "Model, Camera, and Light inherit from SceneNode, providing common positioning, translation, and rotation interfaces.",
        "domain": "scene_objects",
        "week": 1,
        "prerequisites": ["P-SCENE_OBJECTS-002"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/Model.h", "loc": "line 24"},
            {"file": "sources/tl-engine11_headers/Camera.h", "loc": "line 27"},
            {"file": "sources/tl-engine11_headers/Light.h", "loc": "line 29"},
            {"file": "sources/tl-engine11_headers/SceneNode.h", "loc": "lines 21-208"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-SCENE_OBJECTS-006",
        "statement": "World positions are retrieved using GetX(), GetY(), GetZ() and set using SetX(x), SetY(y), SetZ(z), or SetPosition(x, y, z).",
        "domain": "scene_objects",
        "week": 1,
        "prerequisites": ["P-SCENE_OBJECTS-005"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/SceneNode.h", "loc": "lines 39-57"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1B - Introduction to TL Engine and Game Programing.pdf.txt", "loc": "pages 9-10"}
        ],
        "kind": "api"
    },
    {
        "id": "P-SCENE_OBJECTS-007",
        "statement": "World movement translates an object along global Cartesian axes using MoveX(x), MoveY(y), MoveZ(z), or Move(x, y, z).",
        "domain": "scene_objects",
        "week": 1,
        "prerequisites": ["P-SCENE_OBJECTS-005"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/SceneNode.h", "loc": "lines 87-96"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1B - Introduction to TL Engine and Game Programing.pdf.txt", "loc": "page 9"}
        ],
        "kind": "api"
    },
    {
        "id": "P-SCENE_OBJECTS-008",
        "statement": "Local movement translates an object relative to its own rotated orientation using MoveLocalX(x), MoveLocalY(y), MoveLocalZ(z), or MoveLocal(x, y, z).",
        "domain": "scene_objects",
        "week": 1,
        "prerequisites": ["P-SCENE_OBJECTS-005"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/SceneNode.h", "loc": "lines 99-109"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1C - Conditionals and Player Input.pdf.txt", "loc": "pages 4-5"}
        ],
        "kind": "api"
    },
    {
        "id": "P-SCENE_OBJECTS-009",
        "statement": "Rotation turns an object around world axes (RotateX, RotateY, RotateZ) or local axes (RotateLocalX, RotateLocalY, RotateLocalZ) by an angle in degrees.",
        "domain": "scene_objects",
        "week": 1,
        "prerequisites": ["P-SCENE_OBJECTS-005"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/SceneNode.h", "loc": "lines 131-147"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1B - Introduction to TL Engine and Game Programing.pdf.txt", "loc": "page 9"}
        ],
        "kind": "api"
    },
    {
        "id": "P-SCENE_OBJECTS-010",
        "statement": "LookAt(SceneNode* target) or LookAt(x, y, z) reorients a node to face another node or point; ResetOrientation() restores default rotation.",
        "domain": "scene_objects",
        "week": 1,
        "prerequisites": ["P-SCENE_OBJECTS-005"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/SceneNode.h", "loc": "lines 117-123"}
        ],
        "kind": "api"
    },
    {
        "id": "P-SCENE_OBJECTS-011",
        "statement": "Model dimensions can be scaled uniformly with Scale(float scale) or non-uniformly along individual axes using ScaleX, ScaleY, ScaleZ.",
        "domain": "scene_objects",
        "week": 2,
        "prerequisites": ["P-SCENE_OBJECTS-005"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/SceneNode.h", "loc": "lines 154-163"},
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Labs___Lab 2C - Field of Spikes (Game Prototype).pdf.txt", "loc": "pages 3-4"}
        ],
        "kind": "api"
    },
    {
        "id": "P-SCENE_OBJECTS-012",
        "statement": "model->SetSkin(\"filename.jpg\") updates the texture image applied to a model's geometry (applicable to single-texture models).",
        "domain": "scene_objects",
        "week": 2,
        "prerequisites": ["P-SCENE_OBJECTS-002"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/Model.h", "loc": "line 64"},
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Labs___Lab 2A - Texture Manipulation - Mouse Control - Custon Camera Control.pdf.txt", "loc": "page 1"}
        ],
        "kind": "api"
    },
    {
        "id": "P-SCENE_OBJECTS-013",
        "statement": "mesh->RemoveModel(model) destroys the model in the scene; calling methods on the dangling pointer causes a fatal runtime crash.",
        "domain": "scene_objects",
        "week": 3,
        "prerequisites": ["P-SCENE_OBJECTS-002", "P-CPP_BASICS-013"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/Mesh.h", "loc": "line 41"},
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Labs___CO1035 - Lab3A - Finite State Machines.pdf.txt", "loc": "page 2"}
        ],
        "kind": "warning"
    },
    {
        "id": "P-SCENE_OBJECTS-014",
        "statement": "AttachToParent(SceneNode* parent) attaches a child node to a parent node, causing all subsequent movement and rotation of the parent to carry the child.",
        "domain": "scene_objects",
        "week": 3,
        "prerequisites": ["P-SCENE_OBJECTS-005"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/SceneNode.h", "loc": "line 191"},
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Additional Lab___CO1035 - Lab3A-Additional - Platformer Prototype.pdf.txt", "loc": "pages 3-4"}
        ],
        "kind": "api"
    },
    {
        "id": "P-SCENE_OBJECTS-015",
        "statement": "When parented, a child object's origin becomes the parent's origin, requiring local positioning via SetLocalPosition(x, y, z) and queries via GetLocalX/Y/Z().",
        "domain": "scene_objects",
        "week": 3,
        "prerequisites": ["P-SCENE_OBJECTS-014"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/SceneNode.h", "loc": "lines 61-79"},
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Additional Lab___CO1035 - Lab3A-Additional - Platformer Prototype.pdf.txt", "loc": "page 4"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-SCENE_OBJECTS-016",
        "statement": "Standard meshes provided in the TL-Engine media directory include 'Cube.x' (measuring 10x10x10 units), 'Sphere.x', 'Torus.x', 'Grid.x', and 'TwoPence.x'.",
        "domain": "scene_objects",
        "week": 1,
        "prerequisites": ["P-SCENE_OBJECTS-001"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1B - Introduction to TL Engine and Game Programing.pdf.txt", "loc": "pages 7-8"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-SCENE_OBJECTS-017",
        "statement": "Transformation matrices can be accessed directly using GetMatrix(float* matrixElts) and SetMatrix(float* matrixElts), transferring 16 floats for a 4x4 matrix.",
        "domain": "scene_objects",
        "week": 1,
        "prerequisites": ["P-SCENE_OBJECTS-005"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/SceneNode.h", "loc": "lines 176-181"}
        ],
        "kind": "api"
    },
    {
        "id": "P-SCENE_OBJECTS-018",
        "statement": "Scene illumination is managed via SetAmbientLight(r, g, b) on TLEngine and point lights created using CreatePointLight(x, y, z, r, g, b, radius).",
        "domain": "scene_objects",
        "week": 1,
        "prerequisites": ["P-ENGINE_LOOP-002", "P-SCENE_OBJECTS-005"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "lines 136-142"},
            {"file": "sources/tl-engine11_headers/Light.h", "loc": "lines 18-57"}
        ],
        "kind": "api"
    },
    {
        "id": "P-SCENE_OBJECTS-019",
        "statement": "Hierarchical model sub-nodes can be inspected with GetNumNodes() and retrieved as independent SceneNodes via model->GetNode(int node).",
        "domain": "scene_objects",
        "week": 1,
        "prerequisites": ["P-SCENE_OBJECTS-002", "P-SCENE_OBJECTS-005"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/Model.h", "loc": "lines 48-52"}
        ],
        "kind": "api"
    },

    # ---------------------------------------------------------
    # DOMAIN: input
    # ---------------------------------------------------------
    {
        "id": "P-INPUT-001",
        "statement": "Key and button states are represented by KeyState (formerly EKeyState): KeyUp (unpressed), KeyStruck (just pressed on current frame), and KeyDown (held).",
        "domain": "input",
        "week": 1,
        "prerequisites": ["P-CPP_BASICS-011"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/Input.h", "loc": "lines 16-21"},
            {"file": "sources/tl-engine11_headers/LegacyNames.h", "loc": "lines 62-65"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-INPUT-002",
        "statement": "KeyHit(KeyCode keyCode) returns true only on the initial frame a key or mouse button is depressed, making it suitable for single-trigger actions and state toggles.",
        "domain": "input",
        "week": 1,
        "prerequisites": ["P-INPUT-001", "P-ENGINE_LOOP-005"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "lines 200-202"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1C - Conditionals and Player Input.pdf.txt", "loc": "pages 2-3"}
        ],
        "kind": "api"
    },
    {
        "id": "P-INPUT-003",
        "statement": "KeyHeld(KeyCode keyCode) returns true continuously as long as the specified key or mouse button is kept held down, suitable for continuous motion.",
        "domain": "input",
        "week": 1,
        "prerequisites": ["P-INPUT-001", "P-ENGINE_LOOP-005"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "lines 204-207"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1C - Conditionals and Player Input.pdf.txt", "loc": "pages 2-3"}
        ],
        "kind": "api"
    },
    {
        "id": "P-INPUT-004",
        "statement": "Mouse buttons are integrated into the KeyCode enumeration: Mouse_LButton, Mouse_RButton, and Mouse_MButton can be passed to KeyHit and KeyHeld.",
        "domain": "input",
        "week": 1,
        "prerequisites": ["P-INPUT-002", "P-INPUT-003"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/Input.h", "loc": "lines 27-29"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1C - Conditionals and Player Input.pdf.txt", "loc": "pages 2-3"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-INPUT-005",
        "statement": "GetMouseX() and GetMouseY() return the current integer pixel coordinates of the mouse cursor relative to the window's top-left corner.",
        "domain": "input",
        "week": 2,
        "prerequisites": ["P-ENGINE_LOOP-005"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "lines 216-222"},
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Labs___Lab 2A - Texture Manipulation - Mouse Control - Custon Camera Control.pdf.txt", "loc": "page 2"}
        ],
        "kind": "api"
    },
    {
        "id": "P-INPUT-006",
        "statement": "GetMouseMovementX() and GetMouseMovementY() return the relative displacement of the mouse since the last call, used to drive steering and rotation.",
        "domain": "input",
        "week": 2,
        "prerequisites": ["P-ENGINE_LOOP-005"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "lines 228-232"},
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Labs___Lab 2A - Texture Manipulation - Mouse Control - Custon Camera Control.pdf.txt", "loc": "pages 2-3"}
        ],
        "kind": "api"
    },
    {
        "id": "P-INPUT-007",
        "statement": "In TL-Engine 11 headers GetMouseMovementX() and GetMouseMovementY() return float, whereas legacy Appendix 1 documentation lists their return type as int.",
        "domain": "input",
        "week": 2,
        "prerequisites": ["P-INPUT-006"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "lines 228-232"},
            {"file": "sources/text/Setting-up my Development Environemnt__TL-Engine11 Documentation___TL-Engine Appendix 1.pdf.txt", "loc": "page 4"}
        ],
        "kind": "warning"
    },
    {
        "id": "P-INPUT-008",
        "statement": "StartMouseCapture() hides the OS cursor and routes all mouse movements directly to the engine window; StopMouseCapture() restores cursor visibility and normal mouse behavior.",
        "domain": "input",
        "week": 2,
        "prerequisites": ["P-INPUT-006"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "lines 243-248"},
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Labs___Lab 2A - Texture Manipulation - Mouse Control - Custon Camera Control.pdf.txt", "loc": "page 4"}
        ],
        "kind": "api"
    },
    {
        "id": "P-INPUT-009",
        "statement": "AnyKeyHit() and AnyKeyHeld() allow testing if any key or mouse button is currently struck or held down.",
        "domain": "input",
        "week": 1,
        "prerequisites": ["P-INPUT-002", "P-INPUT-003"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "lines 209-213"}
        ],
        "kind": "api"
    },

    # ---------------------------------------------------------
    # DOMAIN: camera
    # ---------------------------------------------------------
    {
        "id": "P-CAMERA-001",
        "statement": "Cameras are created using myEngine->CreateCamera(CameraType cameraType, float x, float y, float z), returning a pointer to Camera (ICamera).",
        "domain": "camera",
        "week": 1,
        "prerequisites": ["P-ENGINE_LOOP-002", "P-SCENE_OBJECTS-005"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "lines 123-124"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1B - Introduction to TL Engine and Game Programing.pdf.txt", "loc": "page 6"}
        ],
        "kind": "api"
    },
    {
        "id": "P-CAMERA-002",
        "statement": "Camera types are ManualCamera (kManual), which remains stationary unless explicitly transformed, and FPSCamera (kFPS), which provides automatic keyboard and mouse navigation.",
        "domain": "camera",
        "week": 1,
        "prerequisites": ["P-CAMERA-001"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/Camera.h", "loc": "lines 17-23"},
            {"file": "sources/tl-engine11_headers/LegacyNames.h", "loc": "lines 44-45"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1C - Conditionals and Player Input.pdf.txt", "loc": "pages 3-4"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-CAMERA-003",
        "statement": "An FPSCamera consumes mouse input internally, interfering with custom mouse interactions unless replaced by a ManualCamera.",
        "domain": "camera",
        "week": 1,
        "prerequisites": ["P-CAMERA-002", "P-INPUT-006"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1C - Conditionals and Player Input.pdf.txt", "loc": "pages 3-4"}
        ],
        "kind": "warning"
    },
    {
        "id": "P-CAMERA-004",
        "statement": "A custom first-person camera is implemented on a ManualCamera by applying MoveLocalX/Y/Z with keys and RotateY/RotateLocalX with mouse movement deltas.",
        "domain": "camera",
        "week": 2,
        "prerequisites": ["P-CAMERA-002", "P-SCENE_OBJECTS-008", "P-SCENE_OBJECTS-009", "P-INPUT-006"],
        "source_refs": [
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Labs___Lab 2A - Texture Manipulation - Mouse Control - Custon Camera Control.pdf.txt", "loc": "pages 3-4"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-CAMERA-005",
        "statement": "A third-person chase camera is created by setting a ManualCamera's position each frame to match a moving model's X and Z coordinates with a fixed offset.",
        "domain": "camera",
        "week": 2,
        "prerequisites": ["P-CAMERA-002", "P-SCENE_OBJECTS-006", "P-ENGINE_LOOP-005"],
        "source_refs": [
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Labs___Lab 2C - Field of Spikes (Game Prototype).pdf.txt", "loc": "pages 2-3"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-CAMERA-006",
        "statement": "Camera viewing parameters are adjusted with SetNearClip(nearDist), SetFarClip(farDist), and SetFOV(fovInDegrees), where typical game field of view is 70 to 100 degrees.",
        "domain": "camera",
        "week": 1,
        "prerequisites": ["P-CAMERA-001"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/Camera.h", "loc": "lines 53-63"}
        ],
        "kind": "api"
    },

    # ---------------------------------------------------------
    # DOMAIN: sprites_ui
    # ---------------------------------------------------------
    {
        "id": "P-SPRITES_UI-001",
        "statement": "Sprites are 2D image bitmaps created via myEngine->CreateSprite(imageName, x, y, z), requiring no mesh blueprint.",
        "domain": "sprites_ui",
        "week": 2,
        "prerequisites": ["P-ENGINE_LOOP-002"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "line 155"},
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Labs___Lab 2B - Drawing Sprites.pdf.txt", "loc": "page 5"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-SPRITES_UI-002",
        "statement": "2D Sprites and Text use a screen pixel coordinate system with origin (0, 0) at top-left, where X increases to the right and Y increases downwards.",
        "domain": "sprites_ui",
        "week": 2,
        "prerequisites": ["P-SPRITES_UI-001"],
        "source_refs": [
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Labs___Lab 2B - Drawing Sprites.pdf.txt", "loc": "page 2"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-SPRITES_UI-003",
        "statement": "Sprite Z depth ranges between 0.0f (drawn closest to screen) and 1.0f (drawn furthest back), used exclusively for layer depth sorting.",
        "domain": "sprites_ui",
        "week": 2,
        "prerequisites": ["P-SPRITES_UI-001"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/Sprite.h", "loc": "line 45"},
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Labs___Lab 2B - Drawing Sprites.pdf.txt", "loc": "page 5"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-SPRITES_UI-004",
        "statement": "Sprites provide 2D positioning and translation via GetX(), GetY(), SetX(), SetY(), SetZ(), SetPosition(), MoveX(), MoveY(), and Move().",
        "domain": "sprites_ui",
        "week": 2,
        "prerequisites": ["P-SPRITES_UI-001", "P-SPRITES_UI-002"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/Sprite.h", "loc": "lines 33-63"},
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Labs___Lab 2B - Drawing Sprites.pdf.txt", "loc": "page 5"}
        ],
        "kind": "api"
    },
    {
        "id": "P-SPRITES_UI-005",
        "statement": "Fonts are loaded via myEngine->LoadFont(fontName, fontSize), returning a Font* (IFont*), and default to 24-point Arial if no font is loaded.",
        "domain": "sprites_ui",
        "week": 2,
        "prerequisites": ["P-ENGINE_LOOP-002"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "line 189"},
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Labs___Lab 2B - Drawing Sprites.pdf.txt", "loc": "page 1"}
        ],
        "kind": "api"
    },
    {
        "id": "P-SPRITES_UI-006",
        "statement": "Text drawn using font->Draw(text, x, y, colour, xAlign, yAlign) persists for a single frame only, and must be called each frame inside the game loop.",
        "domain": "sprites_ui",
        "week": 2,
        "prerequisites": ["P-SPRITES_UI-005", "P-ENGINE_LOOP-005"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/Font.h", "loc": "lines 81-83"},
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Labs___Lab 2B - Drawing Sprites.pdf.txt", "loc": "page 1"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-SPRITES_UI-007",
        "statement": "Font Draw supports horizontal alignment (LeftAlign, CentreAlign, RightAlign) and vertical alignment (TopAlign, MiddleAlign, BottomAlign).",
        "domain": "sprites_ui",
        "week": 2,
        "prerequisites": ["P-SPRITES_UI-006"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/Font.h", "loc": "lines 34-47"},
            {"file": "sources/tl-engine11_headers/LegacyNames.h", "loc": "lines 52-60"},
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Labs___Lab 2B - Drawing Sprites.pdf.txt", "loc": "page 3"}
        ],
        "kind": "api"
    },
    {
        "id": "P-SPRITES_UI-008",
        "statement": "Dynamic text with variables is formatted using <sstream> std::stringstream, rendered with font->Draw(stream.str(), x, y), and cleared each frame with stream.str(\"\").",
        "domain": "sprites_ui",
        "week": 2,
        "prerequisites": ["P-SPRITES_UI-006", "P-CPP_BASICS-002"],
        "source_refs": [
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Labs___Lab 2B - Drawing Sprites.pdf.txt", "loc": "page 4"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-SPRITES_UI-009",
        "statement": "Newline characters ('\\n' or std::endl) have no effect in TL-Engine font->Draw(); multi-line text requires separate Draw calls at different Y coordinates.",
        "domain": "sprites_ui",
        "week": 2,
        "prerequisites": ["P-SPRITES_UI-006"],
        "source_refs": [
            {"file": "sources/text/Setting-up my Development Environemnt__TL-Engine11 Documentation___TL-Engine Appendix 3.pdf.txt", "loc": "page 1"},
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Labs___Lab 2B - Drawing Sprites.pdf.txt", "loc": "page 4"}
        ],
        "kind": "warning"
    },

    # ---------------------------------------------------------
    # DOMAIN: timing
    # ---------------------------------------------------------
    {
        "id": "P-TIMING-001",
        "statement": "FrameTime() returns the duration in seconds between the two most recent calls to DrawScene(), defaulting to 1/60th of a second on startup.",
        "domain": "timing",
        "week": 1,
        "prerequisites": ["P-ENGINE_LOOP-006"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "lines 169-171"}
        ],
        "kind": "api"
    },
    {
        "id": "P-TIMING-002",
        "statement": "Instantaneous frame rate in frames per second equals the reciprocal of frame time: Current FPS = 1.0f / FrameTime().",
        "domain": "timing",
        "week": 1,
        "prerequisites": ["P-TIMING-001"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "line 170"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-TIMING-003",
        "statement": "AverageFPS() returns the rolling average frames per second calculated over the preceding one-second window.",
        "domain": "timing",
        "week": 1,
        "prerequisites": ["P-TIMING-001"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "lines 173-174"}
        ],
        "kind": "api"
    },
    {
        "id": "P-TIMING-004",
        "statement": "EnableVsync(bool enable = true) locks rendering presentation to the monitor's vertical refresh rate (e.g. 60Hz, 120Hz).",
        "domain": "timing",
        "week": 1,
        "prerequisites": ["P-ENGINE_LOOP-006"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "lines 176-177"}
        ],
        "kind": "api"
    },
    {
        "id": "P-TIMING-005",
        "statement": "Timer() returns the total elapsed time in seconds since the 3D engine was initialized.",
        "domain": "timing",
        "week": 1,
        "prerequisites": ["P-ENGINE_LOOP-002"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "lines 258-259"}
        ],
        "kind": "api"
    },
    {
        "id": "P-TIMING-006",
        "statement": "Lab 1B introduced an empty for-loop delay ('for(int delay=0; delay < 1000000; delay++);') to throttle FPS camera speed, which is an antipattern that varies across CPUs.",
        "domain": "timing",
        "week": 1,
        "prerequisites": ["P-CAMERA-002"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1B - Introduction to TL Engine and Game Programing.pdf.txt", "loc": "page 6"}
        ],
        "kind": "warning"
    },
    {
        "id": "P-TIMING-007",
        "statement": "Frame-rate independent movement is achieved by scaling per-frame displacements by delta time dt (retrieved via FrameTime()), ensuring constant speed regardless of frame rate.",
        "domain": "timing",
        "week": 3,
        "prerequisites": ["P-TIMING-001", "P-SCENE_OBJECTS-007"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Lectures___CO1035 - Lecture 3A - Finite State Machines.pptx.txt", "loc": "slide 24"},
            {"file": "sources/tl-engine11_headers/3DEngine.h", "loc": "lines 169-171"}
        ],
        "kind": "procedure"
    },

    # ---------------------------------------------------------
    # DOMAIN: vectors
    # ---------------------------------------------------------
    {
        "id": "P-VECTORS-001",
        "statement": "A scalar is a physical quantity completely defined by magnitude alone (e.g. distance, volume, time, mass, temperature).",
        "domain": "vectors",
        "week": 3,
        "prerequisites": [],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Lectures___CO1035 - Lecture 3B - Math Vectors.pptx.txt", "loc": "slide 6"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-VECTORS-002",
        "statement": "A vector is a physical quantity characterized by both magnitude and direction (e.g. displacement, force, velocity).",
        "domain": "vectors",
        "week": 3,
        "prerequisites": ["P-VECTORS-001"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Lectures___CO1035 - Lecture 3B - Math Vectors.pptx.txt", "loc": "slide 6"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-VECTORS-003",
        "statement": "TL-Engine uses a left-handed Cartesian coordinate system: X is horizontal (thumb), Y is vertical up (index), and Z extends into the scene (middle finger).",
        "domain": "vectors",
        "week": 3,
        "prerequisites": ["P-SCENE_OBJECTS-006"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Lectures___CO1035 - Lecture 3B - Math Vectors.pptx.txt", "loc": "slide 8"},
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1B - Introduction to TL Engine and Game Programing.pdf.txt", "loc": "page 7"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-VECTORS-004",
        "statement": "Vectors are location-independent: two vectors possessing identical magnitude and pointing direction are mathematically equal regardless of their position in space.",
        "domain": "vectors",
        "week": 3,
        "prerequisites": ["P-VECTORS-002"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Lectures___CO1035 - Lecture 3B - Math Vectors.pptx.txt", "loc": "slide 16"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-VECTORS-005",
        "statement": "The vector from initial point P1 to final point P2 is calculated by subtracting initial coordinates from final coordinates: P1P2 = P2 - P1 = (x2-x1, y2-y1, z2-z1).",
        "domain": "vectors",
        "week": 3,
        "prerequisites": ["P-VECTORS-002", "P-VECTORS-004"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Lectures___CO1035 - Lecture 3B - Math Vectors.pptx.txt", "loc": "slides 12-14"},
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Math Exercises___CO1035 - Maths Practice - Vectors.pdf.txt", "loc": "page 2"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-VECTORS-006",
        "statement": "Vector addition is commutative: v + w = w + v = (vx+wx, vy+wy, vz+wz), combining simultaneous influences such as movement velocity and wind.",
        "domain": "vectors",
        "week": 3,
        "prerequisites": ["P-VECTORS-002"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Lectures___CO1035 - Lecture 3B - Math Vectors.pptx.txt", "loc": "slides 17-18"},
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Labs___CO1035 - Lab3B - Math Vectors, Arrays and Loops.pdf.txt", "loc": "pages 2-3"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-VECTORS-007",
        "statement": "Vector subtraction is non-commutative: v - w = (vx-wx, vy-wy, vz-wz), reversing the direction compared to w - v.",
        "domain": "vectors",
        "week": 3,
        "prerequisites": ["P-VECTORS-002"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Lectures___CO1035 - Lecture 3B - Math Vectors.pptx.txt", "loc": "slide 19"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-VECTORS-008",
        "statement": "The length or magnitude of a 3D vector v (denoted ||v||) is computed via 3D Pythagoras' Theorem: ||v|| = sqrt(x^2 + y^2 + z^2).",
        "domain": "vectors",
        "week": 3,
        "prerequisites": ["P-VECTORS-002"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Lectures___CO1035 - Lecture 3B - Math Vectors.pptx.txt", "loc": "slides 20-21"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-VECTORS-009",
        "statement": "A unit or normalized vector (denoted v-hat) has a magnitude of exactly 1, computed by dividing each component by its length: v-hat = (x/||v||, y/||v||, z/||v||).",
        "domain": "vectors",
        "week": 3,
        "prerequisites": ["P-VECTORS-008"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Lectures___CO1035 - Lecture 3B - Math Vectors.pptx.txt", "loc": "slides 25, 29"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-VECTORS-010",
        "statement": "A direction vector is a normalized unit vector representing pure orientation independent of distance and coordinate scale.",
        "domain": "vectors",
        "week": 3,
        "prerequisites": ["P-VECTORS-009"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Lectures___CO1035 - Lecture 3B - Math Vectors.pptx.txt", "loc": "slides 26-28"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-VECTORS-011",
        "statement": "Scalar multiplication scales a vector's length by multiplying each component by scalar k: k * v = (k*x, k*y, k*z), used to convert direction vectors into displacement.",
        "domain": "vectors",
        "week": 3,
        "prerequisites": ["P-VECTORS-001", "P-VECTORS-010"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Lectures___CO1035 - Lecture 3B - Math Vectors.pptx.txt", "loc": "slide 33"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-VECTORS-012",
        "statement": "Mathematical operations adhere to precedence rules (BODMAS / PEMDAS), where multiplication and division hold equal rank, followed by addition and subtraction.",
        "domain": "vectors",
        "week": 3,
        "prerequisites": [],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Lectures___CO1035 - Lecture 3B - Math Vectors.pptx.txt", "loc": "slide 3"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-VECTORS-013",
        "statement": "The official solutions in 'CO1035 - Maths Practice - Vectors.pdf' page 3 contain an arithmetic error in question 2d where 4*(0, 13, 8) is evaluated as (4, 52, 32) instead of (0, 52, 32).",
        "domain": "vectors",
        "week": 3,
        "prerequisites": ["P-VECTORS-011"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Math Exercises___CO1035 - Maths Practice - Vectors.pdf.txt", "loc": "page 3"}
        ],
        "kind": "warning"
    },

    # ---------------------------------------------------------
    # DOMAIN: collision
    # ---------------------------------------------------------
    {
        "id": "P-COLLISION-001",
        "statement": "Bounding spheres encapsulate complex 3D meshes inside simple spherical bounding volumes to simplify intersection calculations.",
        "domain": "collision",
        "week": 3,
        "prerequisites": ["P-VECTORS-008", "P-SCENE_OBJECTS-002"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Additional Lab___CO1035 - Lab3A-Additional - Platformer Prototype.pdf.txt", "loc": "pages 5-6"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-COLLISION-002",
        "statement": "A collision between two spherical bounding volumes occurs when their 3D center distance sqrt((x1-x2)^2 + (y1-y2)^2 + (z1-z2)^2) is less than the sum of their radii (r1 + r2).",
        "domain": "collision",
        "week": 3,
        "prerequisites": ["P-COLLISION-001", "P-VECTORS-008"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Additional Lab___CO1035 - Lab3A-Additional - Platformer Prototype.pdf.txt", "loc": "pages 5-6"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-COLLISION-003",
        "statement": "SceneNode declares EnableCollisions(Model* collider, float gravity = -9.8f, float radiusX = 0.0f, float radiusY = 0.0f, float radiusZ = 0.0f) for built-in collision detection.",
        "domain": "collision",
        "week": 1,
        "prerequisites": ["P-SCENE_OBJECTS-005"],
        "source_refs": [
            {"file": "sources/tl-engine11_headers/SceneNode.h", "loc": "lines 205-207"}
        ],
        "kind": "api"
    },

    # ---------------------------------------------------------
    # DOMAIN: fsm
    # ---------------------------------------------------------
    {
        "id": "P-FSM-001",
        "statement": "A state represents a specific operational condition, status, or mode of behavior in an entity or game system.",
        "domain": "fsm",
        "week": 3,
        "prerequisites": [],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Lectures___CO1035 - Lecture 3A - Finite State Machines.pptx.txt", "loc": "slide 4"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-FSM-002",
        "statement": "A Finite State Machine (FSM) is an abstract computational model consisting of a finite set of states and transitions between them triggered by specific events.",
        "domain": "fsm",
        "week": 3,
        "prerequisites": ["P-FSM-001"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Lectures___CO1035 - Lecture 3A - Finite State Machines.pptx.txt", "loc": "slide 6"},
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Labs___CO1035 - Lab3A - Finite State Machines.pdf.txt", "loc": "page 1"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-FSM-003",
        "statement": "At any given instant, an entity controlled by an FSM can occupy exactly one active state from its finite set.",
        "domain": "fsm",
        "week": 3,
        "prerequisites": ["P-FSM-002"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Labs___CO1035 - Lab3A - Finite State Machines.pdf.txt", "loc": "page 1"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-FSM-004",
        "statement": "In game FSM implementations, code that checks for state transitions (triggered by events) must be structured separately from code that executes per-frame state actions.",
        "domain": "fsm",
        "week": 3,
        "prerequisites": ["P-FSM-002", "P-ENGINE_LOOP-005"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Labs___CO1035 - Lab3A - Finite State Machines.pdf.txt", "loc": "pages 4-5"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-FSM-005",
        "statement": "Binary states (e.g. exists/removed, paused/unpaused) are managed using bool variables, while multi-state systems are managed using enums.",
        "domain": "fsm",
        "week": 3,
        "prerequisites": ["P-FSM-001", "P-CPP_BASICS-003", "P-CPP_BASICS-011"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Labs___CO1035 - Lab3A - Finite State Machines.pdf.txt", "loc": "pages 1-3"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-FSM-006",
        "statement": "State Transition Diagrams (STDs) model FSMs graphically using Start Nodes, State Nodes (specifying state name and activity), Event/Action transition lines, and End Nodes.",
        "domain": "fsm",
        "week": 3,
        "prerequisites": ["P-FSM-002"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Lectures___CO1035 - Lecture 3A - Finite State Machines.pptx.txt", "loc": "slides 8-9"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-FSM-007",
        "statement": "Games often feature multiple interacting FSMs (e.g. game state, player state, NPC AI), where an action in one machine triggers a transition in another.",
        "domain": "fsm",
        "week": 3,
        "prerequisites": ["P-FSM-002"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Lectures___CO1035 - Lecture 3A - Finite State Machines.pptx.txt", "loc": "slides 14-16"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-FSM-008",
        "statement": "FSM classifications include Deterministic (one input maps to one state), Non-Deterministic (one input to multiple states), Probabilistic (transitions weighted by probabilities), and Hierarchical.",
        "domain": "fsm",
        "week": 3,
        "prerequisites": ["P-FSM-002"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Lectures___CO1035 - Lecture 3A - Finite State Machines.pptx.txt", "loc": "slides 17-18"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-FSM-009",
        "statement": "In jump mechanics, intermediate transit states (e.g. MovingUp and MovingDown) prevent player input spamming while transit animations and translations are active.",
        "domain": "fsm",
        "week": 3,
        "prerequisites": ["P-FSM-004"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Additional Lab___CO1035 - Lab3A-Additional - Platformer Prototype.pdf.txt", "loc": "page 5"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-FSM-010",
        "statement": "Lecture 3A slide 24 contains a syntax bug in its transition code snippet, writing 'sphereState == Up;' instead of assignment 'sphereState = Up;'.",
        "domain": "fsm",
        "week": 3,
        "prerequisites": ["P-CPP_BASICS-004", "P-FSM-004"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Lectures___CO1035 - Lecture 3A - Finite State Machines.pptx.txt", "loc": "slide 24"}
        ],
        "kind": "warning"
    },
    {
        "id": "P-FSM-011",
        "statement": "In endless side-scrolling platformers, moving backgrounds and platforms leftwards while recycling them across camera boundaries creates the illusion of continuous player progression.",
        "domain": "fsm",
        "week": 3,
        "prerequisites": ["P-SCENE_OBJECTS-014", "P-SCENE_OBJECTS-015"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Additional Lab___CO1035 - Lab3A-Additional - Platformer Prototype.pdf.txt", "loc": "pages 3-4"}
        ],
        "kind": "procedure"
    },

    # ---------------------------------------------------------
    # DOMAIN: arrays_loops
    # ---------------------------------------------------------
    {
        "id": "P-ARRAYS_LOOPS-001",
        "statement": "A for loop repeats a code block using three control parts: initialization (run once), condition (checked before each iteration), and update (executed after each iteration).",
        "domain": "arrays_loops",
        "week": 1,
        "prerequisites": ["P-CPP_BASICS-006"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1A - Introduction to C .pdf.txt", "loc": "pages 10-11"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-ARRAYS_LOOPS-002",
        "statement": "A while loop tests its continuation condition prior to each iteration, meaning the loop body may execute zero times if initially false.",
        "domain": "arrays_loops",
        "week": 1,
        "prerequisites": ["P-CPP_BASICS-006"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1A - Introduction to C .pdf.txt", "loc": "pages 11-12"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-ARRAYS_LOOPS-003",
        "statement": "A do-while loop evaluates its condition after the body executes, guaranteeing execution at least once and requiring a trailing semicolon after while(cond);.",
        "domain": "arrays_loops",
        "week": 1,
        "prerequisites": ["P-CPP_BASICS-006"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1A - Introduction to C .pdf.txt", "loc": "pages 12-13"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-ARRAYS_LOOPS-004",
        "statement": "Fixed-size C++ arrays store elements of identical data type indexed from 0 to N-1; accessing beyond bounds causes undefined behavior as C++ performs no automatic bounds checks.",
        "domain": "arrays_loops",
        "week": 1,
        "prerequisites": ["P-CPP_BASICS-003"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Labs___Lab 1A - Introduction to C .pdf.txt", "loc": "page 15"}
        ],
        "kind": "warning"
    },
    {
        "id": "P-ARRAYS_LOOPS-005",
        "statement": "Arrays of Model pointers (e.g. IModel* particleModels[kParticles]) allow managing multiple scene objects efficiently inside for-loops for spawning, moving, and recycling.",
        "domain": "arrays_loops",
        "week": 3,
        "prerequisites": ["P-ARRAYS_LOOPS-004", "P-ARRAYS_LOOPS-001", "P-SCENE_OBJECTS-002"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Labs___CO1035 - Lab3B - Math Vectors, Arrays and Loops.pdf.txt", "loc": "pages 3-5"}
        ],
        "kind": "procedure"
    },
    {
        "id": "P-ARRAYS_LOOPS-006",
        "statement": "Parallel arrays maintain associated object attributes across identical indices (e.g. models in particleModels[i], textures in particleSkins[i], states in particleStates[i]).",
        "domain": "arrays_loops",
        "week": 3,
        "prerequisites": ["P-ARRAYS_LOOPS-005", "P-FSM-005"],
        "source_refs": [
            {"file": "sources/text/Week 3 - Finite State Machines and Math Vectors__Labs___CO1035 - Lab3B - Math Vectors, Arrays and Loops.pdf.txt", "loc": "page 6"}
        ],
        "kind": "procedure"
    },

    # ---------------------------------------------------------
    # DOMAIN: game_design
    # ---------------------------------------------------------
    {
        "id": "P-GAME_DESIGN-001",
        "statement": "Commercial game development projects are organized into four sequential phases: Concept Phase, Pre-Production Phase, Production Phase, and Post-Production Phase.",
        "domain": "game_design",
        "week": 1,
        "prerequisites": [],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Lectures___CO1035 - Lecture 1B - The Commercial Games Industry.pptx.txt", "loc": "slide 14"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-GAME_DESIGN-002",
        "statement": "In the Concept Phase, the game idea is conceptualized, visual style and mechanics agreed upon, a concept document produced, and publisher milestone funding sought.",
        "domain": "game_design",
        "week": 1,
        "prerequisites": ["P-GAME_DESIGN-001"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Lectures___CO1035 - Lecture 1B - The Commercial Games Industry.pptx.txt", "loc": "slide 15"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-GAME_DESIGN-003",
        "statement": "In the Pre-Production Phase, technical details and engine are chosen, a playable demo created, team roles assigned, and the full Game Design Document authored.",
        "domain": "game_design",
        "week": 1,
        "prerequisites": ["P-GAME_DESIGN-001"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Lectures___CO1035 - Lecture 1B - The Commercial Games Industry.pptx.txt", "loc": "slide 16"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-GAME_DESIGN-004",
        "statement": "The Production Phase accounts for roughly 50% of the lifecycle, progressing through Alpha (feature complete, final assets added) and Beta (bugs resolved, player testing) to Gold Master release.",
        "domain": "game_design",
        "week": 1,
        "prerequisites": ["P-GAME_DESIGN-001"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Lectures___CO1035 - Lecture 1B - The Commercial Games Industry.pptx.txt", "loc": "slide 17"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-GAME_DESIGN-005",
        "statement": "The Post-Production Phase encompasses post-release support including bug patches, balance updates, and downloadable content (DLC).",
        "domain": "game_design",
        "week": 1,
        "prerequisites": ["P-GAME_DESIGN-001"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Lectures___CO1035 - Lecture 1B - The Commercial Games Industry.pptx.txt", "loc": "slide 18"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-GAME_DESIGN-006",
        "statement": "A High Concept Document is a short summary created in the concept phase containing game title, genre, a one-sentence elevator pitch, core hook, target audience, and key unique features.",
        "domain": "game_design",
        "week": 2,
        "prerequisites": ["P-GAME_DESIGN-002"],
        "source_refs": [
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Lectures___CO1035 - Lecture 2A - Design Documentation.pptx.txt", "loc": "slide 5"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-GAME_DESIGN-007",
        "statement": "A Concept Document expands on the high concept into a structured proposal (2-4 pages for indie, 5-10 for medium, 10-15 for large projects) detailing look, feel, mechanics, and feasibility.",
        "domain": "game_design",
        "week": 2,
        "prerequisites": ["P-GAME_DESIGN-006"],
        "source_refs": [
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Lectures___CO1035 - Lecture 2A - Design Documentation.pptx.txt", "loc": "slides 6-7"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-GAME_DESIGN-008",
        "statement": "A Pitch Document is a concise presentation designed to sell the game to investors or publishers, highlighting market potential, art, timeline, and budget.",
        "domain": "game_design",
        "week": 2,
        "prerequisites": ["P-GAME_DESIGN-006"],
        "source_refs": [
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Lectures___CO1035 - Lecture 2A - Design Documentation.pptx.txt", "loc": "slide 8"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-GAME_DESIGN-009",
        "statement": "The Game Design Document (GDD) is the comprehensive central blueprint ('the game on paper') created during pre-production solely for the internal development team.",
        "domain": "game_design",
        "week": 2,
        "prerequisites": ["P-GAME_DESIGN-003"],
        "source_refs": [
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Lectures___CO1035 - Lecture 2A - Design Documentation.pptx.txt", "loc": "slides 9-10"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-GAME_DESIGN-010",
        "statement": "A GDD standard structure contains 8 core parts: Title Page, Game Overview, Game World, Characters, Objects, User Interface (UI), Play Modes, and Appendices.",
        "domain": "game_design",
        "week": 2,
        "prerequisites": ["P-GAME_DESIGN-009"],
        "source_refs": [
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Lectures___CO1035 - Lecture 2A - Design Documentation.pptx.txt", "loc": "slides 11-12, 32"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-GAME_DESIGN-011",
        "statement": "In a GDD, the User Interface section covers three distinct elements: On-Screen Display (OSD), camera/point-of-view, and controller input interface.",
        "domain": "game_design",
        "week": 2,
        "prerequisites": ["P-GAME_DESIGN-010"],
        "source_refs": [
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Lectures___CO1035 - Lecture 2A - Design Documentation.pptx.txt", "loc": "slide 25"}
        ],
        "kind": "definition"
    },

    # ---------------------------------------------------------
    # DOMAIN: industry
    # ---------------------------------------------------------
    {
        "id": "P-INDUSTRY-001",
        "statement": "The commercial games industry comprises four major key player types: Hardware/Platform Manufacturers, Game Developers/Studios, Publishers, and Retailers.",
        "domain": "industry",
        "week": 1,
        "prerequisites": [],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Lectures___CO1035 - Lecture 1B - The Commercial Games Industry.pptx.txt", "loc": "slides 4-8"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-INDUSTRY-002",
        "statement": "Game development studios range across AAA (large teams, high budgets, global scale), AA (medium scale), Indie (small, independent or self-funded), and Outsourcing/Support studios.",
        "domain": "industry",
        "week": 1,
        "prerequisites": ["P-INDUSTRY-001"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Lectures___CO1035 - Lecture 1B - The Commercial Games Industry.pptx.txt", "loc": "slide 6"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-INDUSTRY-003",
        "statement": "Publishers finance game production and marketing, monitor project milestones, and oversee retail and digital distribution.",
        "domain": "industry",
        "week": 1,
        "prerequisites": ["P-INDUSTRY-001"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Lectures___CO1035 - Lecture 1B - The Commercial Games Industry.pptx.txt", "loc": "slide 7"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-INDUSTRY-004",
        "statement": "Retailers act as the direct point of sale to consumers, operating through physical stores (e.g. GameStop) or digital storefronts (e.g. Steam, PlayStation Store, Nintendo eShop).",
        "domain": "industry",
        "week": 1,
        "prerequisites": ["P-INDUSTRY-001"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Lectures___CO1035 - Lecture 1B - The Commercial Games Industry.pptx.txt", "loc": "slide 8"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-INDUSTRY-005",
        "statement": "A game development team typically subdivides into four functional groups: Management (Producers), Programmers (mechanics/logic), Artists (visual assets/animations), and Designers (gameplay rules).",
        "domain": "industry",
        "week": 1,
        "prerequisites": ["P-INDUSTRY-002"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Lectures___CO1035 - Lecture 1B - The Commercial Games Industry.pptx.txt", "loc": "slide 21"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-INDUSTRY-006",
        "statement": "Game designer roles include Gameplay Designers (core interactions), Level Designers (environments and pacing), Narrative Designers (story and dialogue), and System Designers (balance and economy).",
        "domain": "industry",
        "week": 1,
        "prerequisites": ["P-INDUSTRY-005"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Lectures___CO1035 - Lecture 1B - The Commercial Games Industry.pptx.txt", "loc": "slide 25"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-INDUSTRY-007",
        "statement": "UCLan Cyprus operates 'Rays and Vectors', a student-led game studio based at the InSPIRE lab that simulates commercial workplace conditions and role rotation.",
        "domain": "industry",
        "week": 1,
        "prerequisites": ["P-INDUSTRY-005"],
        "source_refs": [
            {"file": "sources/text/Week 1 - Introduction and the Commercial Game Development Industry__Lectures___CO1035 - Lecture 1A - Introduction to the Module.pptx.txt", "loc": "slides 6-7"}
        ],
        "kind": "fact"
    },

    # ---------------------------------------------------------
    # DOMAIN: ethics_ratings
    # ---------------------------------------------------------
    {
        "id": "P-ETHICS_RATINGS-001",
        "statement": "Video games are regulated through censorship (the modification or restriction of content by governments, publishers, or platforms) and rating systems (age classification).",
        "domain": "ethics_ratings",
        "week": 2,
        "prerequisites": [],
        "source_refs": [
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Lectures___CO1035 - Lecture 2B - Censorship and Ratings System.pptx.txt", "loc": "slides 3-4"}
        ],
        "kind": "definition"
    },
    {
        "id": "P-ETHICS_RATINGS-002",
        "statement": "Commonly censored video game content includes extreme violence, nudity and sexual content, political or ideological references, religious symbolism, and drug or gambling themes.",
        "domain": "ethics_ratings",
        "week": 2,
        "prerequisites": ["P-ETHICS_RATINGS-001"],
        "source_refs": [
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Lectures___CO1035 - Lecture 2B - Censorship and Ratings System.pptx.txt", "loc": "slide 4"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-ETHICS_RATINGS-003",
        "statement": "PEGI (Pan European Game Information), established in 2003, is the primary age classification system across the EU and UK with categories 3, 7, 12, 16, and 18.",
        "domain": "ethics_ratings",
        "week": 2,
        "prerequisites": ["P-ETHICS_RATINGS-001"],
        "source_refs": [
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Lectures___CO1035 - Lecture 2B - Censorship and Ratings System.pptx.txt", "loc": "slides 8-9"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-ETHICS_RATINGS-004",
        "statement": "In the UK, the BBFC historically classified video games until 2012, when game classification was transferred entirely to PEGI.",
        "domain": "ethics_ratings",
        "week": 2,
        "prerequisites": ["P-ETHICS_RATINGS-003"],
        "source_refs": [
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Lectures___CO1035 - Lecture 2B - Censorship and Ratings System.pptx.txt", "loc": "slide 11"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-ETHICS_RATINGS-005",
        "statement": "ESRB (Entertainment Software Rating Board), founded in 1994, is the rating authority for North America with categories eC, E, E10+, T, M, and AO, alongside content descriptors.",
        "domain": "ethics_ratings",
        "week": 2,
        "prerequisites": ["P-ETHICS_RATINGS-001"],
        "source_refs": [
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Lectures___CO1035 - Lecture 2B - Censorship and Ratings System.pptx.txt", "loc": "slides 8, 12-13"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-ETHICS_RATINGS-006",
        "statement": "Other major international classification boards include CERO in Japan, USK in Germany, ACB in Australia, and IARC for digital storefronts.",
        "domain": "ethics_ratings",
        "week": 2,
        "prerequisites": ["P-ETHICS_RATINGS-001"],
        "source_refs": [
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Lectures___CO1035 - Lecture 2B - Censorship and Ratings System.pptx.txt", "loc": "slide 8"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-ETHICS_RATINGS-007",
        "statement": "Cyprus officially supports PEGI, but has no specific domestic legislative basis or mandatory statutory rating enforcement system in place.",
        "domain": "ethics_ratings",
        "week": 2,
        "prerequisites": ["P-ETHICS_RATINGS-003"],
        "source_refs": [
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Lectures___CO1035 - Lecture 2B - Censorship and Ratings System.pptx.txt", "loc": "slide 9"}
        ],
        "kind": "fact"
    },
    {
        "id": "P-ETHICS_RATINGS-008",
        "statement": "Rating criteria frequently produce inconsistencies across boards for identical titles (e.g. The Sims 4 rated T by ESRB, 12 by PEGI, 6+ by USK, and 18+ by Russia's RARS).",
        "domain": "ethics_ratings",
        "week": 2,
        "prerequisites": ["P-ETHICS_RATINGS-003", "P-ETHICS_RATINGS-005", "P-ETHICS_RATINGS-006"],
        "source_refs": [
            {"file": "sources/text/Week 2 - Game Design Documentation, Censorship in Games and Rating Systems__Lectures___CO1035 - Lecture 2B - Censorship and Ratings System.pptx.txt", "loc": "slide 15"}
        ],
        "kind": "fact"
    }
]

REASONING_CHAINS = [
    {
        "id": "C-001",
        "premise_ids": ["P-SETUP-001", "P-SETUP-002", "P-SETUP-003"],
        "conclusion_id": "P-SETUP-008",
        "relation": "enables",
        "note": "Installing Visual Studio with C++ workloads and TL-Engine enables compiling and executing the project via Local Windows Debugger."
    },
    {
        "id": "C-002",
        "premise_ids": ["P-ENGINE_LOOP-001", "P-CPP_BASICS-001"],
        "conclusion_id": "P-ENGINE_LOOP-002",
        "relation": "enables",
        "note": "Including engine headers and main() function enables invoking New3DEngine() to instantiate the 3D graphics engine pointer."
    },
    {
        "id": "C-003",
        "premise_ids": ["P-ENGINE_LOOP-002", "P-ENGINE_LOOP-003"],
        "conclusion_id": "P-ENGINE_LOOP-005",
        "relation": "enables",
        "note": "Instantiating the engine pointer and opening the window via StartWindowed enables running the while(myEngine->IsRunning()) game loop."
    },
    {
        "id": "C-004",
        "premise_ids": ["P-ENGINE_LOOP-005"],
        "conclusion_id": "P-ENGINE_LOOP-006",
        "relation": "enables",
        "note": "The active IsRunning() game loop enables per-frame scene rendering via DrawScene()."
    },
    {
        "id": "C-005",
        "premise_ids": ["P-ENGINE_LOOP-009", "P-ENGINE_LOOP-010"],
        "conclusion_id": "P-SCENE_OBJECTS-001",
        "relation": "enables",
        "note": "Registering media folders via AddMediaFolder enables LoadMesh() to locate and parse .x geometry files on disk."
    },
    {
        "id": "C-006",
        "premise_ids": ["P-SCENE_OBJECTS-001"],
        "conclusion_id": "P-SCENE_OBJECTS-002",
        "relation": "enables",
        "note": "Loading a Mesh blueprint into memory enables instantiating concrete Model instances in the scene using CreateModel()."
    },
    {
        "id": "C-007",
        "premise_ids": ["P-SCENE_OBJECTS-001", "P-SCENE_OBJECTS-002"],
        "conclusion_id": "P-SCENE_OBJECTS-003",
        "relation": "explains",
        "note": "The distinction between Mesh (blueprint) and Model (instance) explains why multiple models share the same loaded mesh without redundant disk reads."
    },
    {
        "id": "C-008",
        "premise_ids": ["P-SCENE_OBJECTS-005"],
        "conclusion_id": "P-CAMERA-004",
        "relation": "explains",
        "note": "Camera inheriting from SceneNode explains why cameras can be moved and rotated using the exact same methods (MoveLocal, RotateX, RotateY) as 3D models."
    },
    {
        "id": "C-009",
        "premise_ids": ["P-INPUT-002", "P-INPUT-003"],
        "conclusion_id": "P-CPP_BASICS-006",
        "relation": "explains",
        "note": "KeyHit triggering once on press versus KeyHeld triggering continuously explains how conditional if-branches distinguish toggles from continuous movement."
    },
    {
        "id": "C-010",
        "premise_ids": ["P-INPUT-006", "P-SCENE_OBJECTS-009"],
        "conclusion_id": "P-CAMERA-004",
        "relation": "enables",
        "note": "Querying horizontal and vertical mouse deltas enables steering model and custom manual camera rotations."
    },
    {
        "id": "C-011",
        "premise_ids": ["P-INPUT-008", "P-INPUT-006"],
        "conclusion_id": "P-CAMERA-004",
        "relation": "explains",
        "note": "Mouse capture via StartMouseCapture() explains how custom camera steering operates continuously without the cursor leaving the viewport."
    },
    {
        "id": "C-012",
        "premise_ids": ["P-CAMERA-002", "P-CAMERA-003"],
        "conclusion_id": "P-CAMERA-004",
        "relation": "enables",
        "note": "Selecting a ManualCamera isolates camera control from automatic mouse capture, enabling custom user-programmed camera behaviors."
    },
    {
        "id": "C-013",
        "premise_ids": ["P-SCENE_OBJECTS-006", "P-ENGINE_LOOP-005"],
        "conclusion_id": "P-CAMERA-005",
        "relation": "enables",
        "note": "Tracking model world positions in the game loop enables implementing a third-person chase camera maintaining fixed relative offsets."
    },
    {
        "id": "C-014",
        "premise_ids": ["P-SPRITES_UI-002", "P-CAMERA-004"],
        "conclusion_id": "P-SPRITES_UI-006",
        "relation": "explains",
        "note": "2D screen pixel space being independent of 3D world space explains why camera rotations do not alter HUD text and sprite positions."
    },
    {
        "id": "C-015",
        "premise_ids": ["P-SPRITES_UI-006", "P-ENGINE_LOOP-005"],
        "conclusion_id": "P-SPRITES_UI-008",
        "relation": "explains",
        "note": "Because font->Draw() text only renders for a single frame, stringstream-based UI displays must be reformatted and drawn per-frame inside the game loop."
    },
    {
        "id": "C-016",
        "premise_ids": ["P-VECTORS-002", "P-VECTORS-004"],
        "conclusion_id": "P-VECTORS-005",
        "relation": "explains",
        "note": "Vectors having magnitude and direction independent of position explains why subtracting initial from target coordinates yields the displacement vector."
    },
    {
        "id": "C-017",
        "premise_ids": ["P-VECTORS-003", "P-VECTORS-005"],
        "conclusion_id": "P-VECTORS-008",
        "relation": "enables",
        "note": "3D Cartesian coordinate decomposition and Pythagoras' Theorem enable computing vector magnitude (length) from displacement components."
    },
    {
        "id": "C-018",
        "premise_ids": ["P-VECTORS-008", "P-VECTORS-009"],
        "conclusion_id": "P-VECTORS-010",
        "relation": "enables",
        "note": "Dividing a displacement vector by its Pythagorean length enables creating a pure scale-independent direction vector of length 1."
    },
    {
        "id": "C-019",
        "premise_ids": ["P-VECTORS-010", "P-VECTORS-011"],
        "conclusion_id": "P-SCENE_OBJECTS-007",
        "relation": "enables",
        "note": "Multiplying a normalized direction vector by a distance scalar enables moving objects at exact controlled speeds in world space."
    },
    {
        "id": "C-020",
        "premise_ids": ["P-VECTORS-008", "P-COLLISION-001"],
        "conclusion_id": "P-COLLISION-002",
        "relation": "enables",
        "note": "Calculating Euclidean distance between object centers enables evaluating whether two bounding spheres intersect."
    },
    {
        "id": "C-021",
        "premise_ids": ["P-SCENE_OBJECTS-013", "P-CPP_BASICS-013"],
        "conclusion_id": "P-FSM-005",
        "relation": "explains",
        "note": "Destroyed models leaving dangling pointers explains why state flags (such as bool sphereExists) are required to guard against calling methods on null objects."
    },
    {
        "id": "C-022",
        "premise_ids": ["P-FSM-005", "P-CPP_BASICS-011"],
        "conclusion_id": "P-FSM-002",
        "relation": "refines",
        "note": "Enumerations refine simple binary boolean flags into multi-state Finite State Machines with distinct named states and clear transitions."
    },
    {
        "id": "C-023",
        "premise_ids": ["P-FSM-004", "P-ENGINE_LOOP-005"],
        "conclusion_id": "P-FSM-002",
        "relation": "explains",
        "note": "Decoupling transition checking from action execution explains how continuous state behavior executes across multiple frames while awaiting triggers."
    },
    {
        "id": "C-024",
        "premise_ids": ["P-SCENE_OBJECTS-014"],
        "conclusion_id": "P-SCENE_OBJECTS-015",
        "relation": "explains",
        "note": "AttachToParent() shifting a child object's origin to the parent's origin explains why SetLocalPosition() must be used to position attached children."
    },
    {
        "id": "C-025",
        "premise_ids": ["P-ARRAYS_LOOPS-004", "P-ARRAYS_LOOPS-001"],
        "conclusion_id": "P-ARRAYS_LOOPS-005",
        "relation": "enables",
        "note": "Fixed-size arrays and for-loop counters enable iterating over groups of model pointers to update and reset particle systems."
    },
    {
        "id": "C-026",
        "premise_ids": ["P-ARRAYS_LOOPS-005", "P-FSM-005"],
        "conclusion_id": "P-ARRAYS_LOOPS-006",
        "relation": "enables",
        "note": "Combining model pointer arrays with state and skin arrays enables parallel array architectures for per-entity state tracking."
    },
    {
        "id": "C-027",
        "premise_ids": ["P-TIMING-001", "P-TIMING-006"],
        "conclusion_id": "P-TIMING-007",
        "relation": "refines",
        "note": "Querying actual elapsed seconds via FrameTime() refines crude busy-wait empty delay loops into professional frame-rate independent physics."
    },
    {
        "id": "C-028",
        "premise_ids": ["P-ENGINE_LOOP-002", "P-SCENE_OBJECTS-001"],
        "conclusion_id": "P-INPUT-007",
        "relation": "refines",
        "note": "Modern TL-Engine 11 header definitions refine legacy lab type names (I3DEngine, IMesh) and return signatures (float vs int for mouse deltas)."
    },
    {
        "id": "C-029",
        "premise_ids": ["P-ASSESSMENT-004"],
        "conclusion_id": "P-ASSESSMENT-005",
        "relation": "contradicts",
        "note": "The assessment structure presented in Lecture 1A (two programming assignments: 40% and 60%) contradicts the formal module descriptor (portfolio and in-class maths test)."
    },
    {
        "id": "C-030",
        "premise_ids": ["P-ASSESSMENT-006"],
        "conclusion_id": "P-ASSESSMENT-007",
        "relation": "explains",
        "note": "The presence of only the Blackboard description page without the attached brief files confirms that the exact Assignment 1 requirements are an unknown gap."
    },
    {
        "id": "C-031",
        "premise_ids": ["P-GAME_DESIGN-001", "P-GAME_DESIGN-002", "P-GAME_DESIGN-003"],
        "conclusion_id": "P-GAME_DESIGN-009",
        "relation": "explains",
        "note": "The progression of development phases explains why High Concept and Pitch documents are created first, followed by the complete GDD in pre-production."
    },
    {
        "id": "C-032",
        "premise_ids": ["P-GAME_DESIGN-009"],
        "conclusion_id": "P-GAME_DESIGN-010",
        "relation": "explains",
        "note": "The GDD serving as the sole comprehensive blueprint for the development team explains its structured breakdown into World, Characters, Objects, UI, and Modes."
    },
    {
        "id": "C-033",
        "premise_ids": ["P-ETHICS_RATINGS-001", "P-ETHICS_RATINGS-002"],
        "conclusion_id": "P-ETHICS_RATINGS-008",
        "relation": "explains",
        "note": "National legal and cultural differences in censoring violence, sex, and drugs explain why classification ratings diverge between PEGI, ESRB, and USK."
    },
    {
        "id": "C-034",
        "premise_ids": ["P-CPP_BASICS-004"],
        "conclusion_id": "P-FSM-010",
        "relation": "explains",
        "note": "The distinction between assignment (=) and equality comparison (==) explains the syntax bug in Lecture 3A slide 24 where state is compared instead of assigned."
    },
    {
        "id": "C-035",
        "premise_ids": ["P-VECTORS-011"],
        "conclusion_id": "P-VECTORS-013",
        "relation": "contradicts",
        "note": "The vector exercise solution 4*(0, 13, 8) = (4, 52, 32) in Maths Practice Vectors contradicts scalar multiplication rules where 4 * 0 must equal 0."
    }
]

def main():
    root = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
    wm_dir = os.path.join(root, "world_model")
    os.makedirs(wm_dir, exist_ok=True)

    props_path = os.path.join(wm_dir, "propositions.jsonl")
    with open(props_path, "w", encoding="utf-8") as f:
        for p in PROPOSITIONS:
            f.write(json.dumps(p, ensure_ascii=False) + "\n")
    print(f"Wrote {len(PROPOSITIONS)} propositions to {props_path}")

    chains_path = os.path.join(wm_dir, "reasoning_chains.jsonl")
    with open(chains_path, "w", encoding="utf-8") as f:
        for c in REASONING_CHAINS:
            f.write(json.dumps(c, ensure_ascii=False) + "\n")
    print(f"Wrote {len(REASONING_CHAINS)} reasoning chains to {chains_path}")

if __name__ == "__main__":
    main()
