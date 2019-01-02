**About arc42**

arc42, the Template for documentation of software and system
architecture.

By Dr. Gernot Starke, Dr. Peter Hruschka and contributors.

Template Revision: 7.0 EN (based on asciidoc), January 2017

© We acknowledge that this document uses material from the arc 42
architecture template, <http://www.arc42.de>. Created by Dr. Peter
Hruschka & Dr. Gernot Starke.

Introduction and Goals {#section-introduction-and-goals}
======================

Requirements Overview {#_requirements_overview}
---------------------

Quality Goals {#_quality_goals}
-------------

* **Extensibility:** The project is meant to start with a very narrow scope and limited requirements. Felxibility is mandatory, to add more and more typical functionality of modern email (fat-)clients.
* **Modularity:** At least the IMAP related functionality must be realized as a reusable library.
* **Robustness:** The application must deal well with all types of protocol and network connection issues with manifestation in well designed interfaces. Goal is to be able to provide excellent user experience in error situations.

Stakeholders {#_stakeholders}
------------

+-------------+---------------------------+---------------------------+
| Role/Name   | Contact                   | Expectations              |
+=============+===========================+===========================+
| *Steffen Templin | *steffen.templin@gmail.com*           | *learn C++ 11 and practice arc42*       |
| *           |                           |                           |
+-------------+---------------------------+---------------------------+

Architecture Constraints {#section-architecture-constraints}
========================

* C++ 11 must be used
* The IMAP protocol and connection implementation must be created from scratch and not come from an external library. Parts like MIME on the other hand might be used from external libraries.

System Scope and Context {#section-system-scope-and-context}
========================

Business Context {#_business_context}
----------------

**\<Diagram or Table\>**

**\<optionally: Explanation of external domain interfaces\>**

Technical Context {#_technical_context}
-----------------

**\<Diagram or Table\>**

**\<optionally: Explanation of technical interfaces\>**

**\<Mapping Input/Output to Channels\>**

Solution Strategy {#section-solution-strategy}
=================

Building Block View {#section-building-block-view}
===================

Whitebox Overall System {#_whitebox_overall_system}
-----------------------

***\<Overview Diagram\>***

Motivation

:   *\<text explanation\>*

Contained Building Blocks

:   *\<Description of contained building block (black boxes)\>*

Important Interfaces

:   *\<Description of important interfaces\>*

### \<Name black box 1\> {#_name_black_box_1}

*\<Purpose/Responsibility\>*

*\<Interface(s)\>*

*\<(Optional) Quality/Performance Characteristics\>*

*\<(Optional) Directory/File Location\>*

*\<(Optional) Fulfilled Requirements\>*

*\<(optional) Open Issues/Problems/Risks\>*

### \<Name black box 2\> {#_name_black_box_2}

*\<black box template\>*

### \<Name black box n\> {#_name_black_box_n}

*\<black box template\>*

### \<Name interface 1\> {#_name_interface_1}

...​

### \<Name interface m\> {#_name_interface_m}

Level 2 {#_level_2}
-------

### White Box *\<building block 1\>* {#_white_box_building_block_1}

*\<white box template\>*

### White Box *\<building block 2\>* {#_white_box_building_block_2}

*\<white box template\>*

...​

### White Box *\<building block m\>* {#_white_box_building_block_m}

*\<white box template\>*

Level 3 {#_level_3}
-------

### White Box \<\_building block x.1\_\> {#_white_box_building_block_x_1}

*\<white box template\>*

### White Box \<\_building block x.2\_\> {#_white_box_building_block_x_2}

*\<white box template\>*

### White Box \<\_building block y.1\_\> {#_white_box_building_block_y_1}

*\<white box template\>*

Runtime View {#section-runtime-view}
============

\<Runtime Scenario 1\> {#_runtime_scenario_1}
----------------------

-   *\<insert runtime diagram or textual description of the scenario\>*

-   *\<insert description of the notable aspects of the interactions
    between the building block instances depicted in this diagram.\>*

\<Runtime Scenario 2\> {#_runtime_scenario_2}
----------------------

...​
----

\<Runtime Scenario n\> {#_runtime_scenario_n}
----------------------

Deployment View {#section-deployment-view}
===============

Infrastructure Level 1 {#_infrastructure_level_1}
----------------------

***\<Overview Diagram\>***

Motivation

:   *\<explanation in text form\>*

Quality and/or Performance Features

:   *\<explanation in text form\>*

Mapping of Building Blocks to Infrastructure

:   *\<description of the mapping\>*

Infrastructure Level 2 {#_infrastructure_level_2}
----------------------

### *\<Infrastructure Element 1\>* {#_infrastructure_element_1}

*\<diagram + explanation\>*

### *\<Infrastructure Element 2\>* {#_infrastructure_element_2}

*\<diagram + explanation\>*

...​

### *\<Infrastructure Element n\>* {#_infrastructure_element_n}

*\<diagram + explanation\>*

Cross-cutting Concepts {#section-concepts}
======================

*\<Concept 1\>* {#_concept_1}
---------------

*\<explanation\>*

*\<Concept 2\>* {#_concept_2}
---------------

*\<explanation\>*

...​

*\<Concept n\>* {#_concept_n}
---------------

*\<explanation\>*

Design Decisions {#section-design-decisions}
================

Quality Requirements {#section-quality-scenarios}
====================

Quality Tree {#_quality_tree}
------------

Quality Scenarios {#_quality_scenarios}
-----------------

Risks and Technical Debts {#section-technical-risks}
=========================

Glossary {#section-glossary}
========

+-----------------------------------+-----------------------------------+
| Term                              | Definition                        |
+===================================+===================================+
| \<Term-1\>                        | \<definition-1\>                  |
+-----------------------------------+-----------------------------------+
| \<Term-2\>                        | \<definition-2\>                  |
+-----------------------------------+-----------------------------------+
