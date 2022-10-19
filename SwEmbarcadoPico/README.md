# TensorFlow Lite Micro

An Open Source Machine Learning Framework for Everyone.

## Introduction

This is a version of the [TensorFlow Lite Micro library](https://www.tensorflow.org/lite/microcontrollers)
for the Raspberry Pi Pico microcontroller. It allows you to run machine learning models to
do things like voice recognition, detect people in images, recognize gestures from an accelerometer,
and other sensor analysis tasks.

## Getting Started

First you'll need to follow the Pico setup instructions to initialize the development
environment on your machine. Once that is done, make sure that the PICO_SDK_PATH
environment variable has been set to the location of the Pico SDK, either in the shell
you're building in, or the CMake configure environment variable setting of the extension
if you're using VS Code.

You should then be able to build the library, tests, and examples. The easiest way to
build is using VS Code's CMake integration, by loading the project and choosing the
build option at the bottom of the window.

## What's Included

There are several example applications included. The simplest one to begin with is the
hello_world project. This demonstrates the fundamentals of deploying an ML model on a
device, driving the Pico's LED in a learned sine-wave pattern.

Other examples include simple speech recognition, a magic wand gesture recognizer,
and spotting people in camera images, but because they require audio, accelerometer or
image inputs you'll need to write some code to hook up your own sensors, since these
are not included with the base microcontroller.

## Contributing

This repository (https://github.com/raspberrypi/pico-tflmicro) is read-only, because
it has been automatically generated from the master TensorFlow repository at
https://github.com/tensorflow/tensorflow. This means that all issues and pull requests
need to be filed there. You can generate a version of this generated project by
running the commands:

```
git clone https://github.com/tensorflow/tensorflow
cd tensorflow
tensorflow/lite/micro/tools/project/generate.py rp2 pico-tflmicro
```

This should create a Pico-compatible project from the latest version of the TensorFlow
repository.

## Learning More

The [TensorFlow website](https://www.tensorflow.org/lite/microcontrollers) has
information on training, tutorials, and other resources.

The [TinyML Book](https://tinymlbook.com) is a guide to using TensorFlow Lite Micro
across a variety of different systems.

[TensorFlowLite Micro: Embedded Machine Learning on TinyML Systems](https://arxiv.org/pdf/2010.08678.pdf)
has more details on the design and implementation of the framework.

## Licensing

The TensorFlow source code is covered by the license described in src/tensorflow/LICENSE,
components from other libraries have the appropriate licenses included in their
third_party folders.

=======
# pico-tflmicro



## Getting started

To make it easy for you to get started with GitLab, here's a list of recommended next steps.

Already a pro? Just edit this README.md and make it your own. Want to make it easy? [Use the template at the bottom](#editing-this-readme)!

## Add your files

- [ ] [Create](https://docs.gitlab.com/ee/user/project/repository/web_editor.html#create-a-file) or [upload](https://docs.gitlab.com/ee/user/project/repository/web_editor.html#upload-a-file) files
- [ ] [Add files using the command line](https://docs.gitlab.com/ee/gitlab-basics/add-file.html#add-a-file-using-the-command-line) or push an existing Git repository with the following command:

```
cd existing_repo
git remote add origin https://gitlab.com/leomar_mateus/pico-tflmicro.git
git branch -M main
git push -uf origin main
```

## Integrate with your tools

- [ ] [Set up project integrations](https://gitlab.com/leomar_mateus/pico-tflmicro/-/settings/integrations)

## Collaborate with your team

- [ ] [Invite team members and collaborators](https://docs.gitlab.com/ee/user/project/members/)
- [ ] [Create a new merge request](https://docs.gitlab.com/ee/user/project/merge_requests/creating_merge_requests.html)
- [ ] [Automatically close issues from merge requests](https://docs.gitlab.com/ee/user/project/issues/managing_issues.html#closing-issues-automatically)
- [ ] [Enable merge request approvals](https://docs.gitlab.com/ee/user/project/merge_requests/approvals/)
- [ ] [Automatically merge when pipeline succeeds](https://docs.gitlab.com/ee/user/project/merge_requests/merge_when_pipeline_succeeds.html)

## Test and Deploy

Use the built-in continuous integration in GitLab.

- [ ] [Get started with GitLab CI/CD](https://docs.gitlab.com/ee/ci/quick_start/index.html)
- [ ] [Analyze your code for known vulnerabilities with Static Application Security Testing(SAST)](https://docs.gitlab.com/ee/user/application_security/sast/)
- [ ] [Deploy to Kubernetes, Amazon EC2, or Amazon ECS using Auto Deploy](https://docs.gitlab.com/ee/topics/autodevops/requirements.html)
- [ ] [Use pull-based deployments for improved Kubernetes management](https://docs.gitlab.com/ee/user/clusters/agent/)
- [ ] [Set up protected environments](https://docs.gitlab.com/ee/ci/environments/protected_environments.html)

***

# Editing this README

When you're ready to make this README your own, just edit this file and use the handy template below (or feel free to structure it however you want - this is just a starting point!).  Thank you to [makeareadme.com](https://www.makeareadme.com/) for this template.

## Suggestions for a good README
Every project is different, so consider which of these sections apply to yours. The sections used in the template are suggestions for most open source projects. Also keep in mind that while a README can be too long and detailed, too long is better than too short. If you think your README is too long, consider utilizing another form of documentation rather than cutting out information.

## Name
Choose a self-explaining name for your project.

## Description
Let people know what your project can do specifically. Provide context and add a link to any reference visitors might be unfamiliar with. A list of Features or a Background subsection can also be added here. If there are alternatives to your project, this is a good place to list differentiating factors.

## Badges
On some READMEs, you may see small images that convey metadata, such as whether or not all the tests are passing for the project. You can use Shields to add some to your README. Many services also have instructions for adding a badge.

## Visuals
Depending on what you are making, it can be a good idea to include screenshots or even a video (you'll frequently see GIFs rather than actual videos). Tools like ttygif can help, but check out Asciinema for a more sophisticated method.

## Installation
Within a particular ecosystem, there may be a common way of installing things, such as using Yarn, NuGet, or Homebrew. However, consider the possibility that whoever is reading your README is a novice and would like more guidance. Listing specific steps helps remove ambiguity and gets people to using your project as quickly as possible. If it only runs in a specific context like a particular programming language version or operating system or has dependencies that have to be installed manually, also add a Requirements subsection.

## Usage
Use examples liberally, and show the expected output if you can. It's helpful to have inline the smallest example of usage that you can demonstrate, while providing links to more sophisticated examples if they are too long to reasonably include in the README.

## Support
Tell people where they can go to for help. It can be any combination of an issue tracker, a chat room, an email address, etc.

## Roadmap
If you have ideas for releases in the future, it is a good idea to list them in the README.

## Contributing
State if you are open to contributions and what your requirements are for accepting them.

For people who want to make changes to your project, it's helpful to have some documentation on how to get started. Perhaps there is a script that they should run or some environment variables that they need to set. Make these steps explicit. These instructions could also be useful to your future self.

You can also document commands to lint the code or run tests. These steps help to ensure high code quality and reduce the likelihood that the changes inadvertently break something. Having instructions for running tests is especially helpful if it requires external setup, such as starting a Selenium server for testing in a browser.

## Authors and acknowledgment
Show your appreciation to those who have contributed to the project.

## License
For open source projects, say how it is licensed.

## Project status
If you have run out of energy or time for your project, put a note at the top of the README saying that development has slowed down or stopped completely. Someone may choose to fork your project or volunteer to step in as a maintainer or owner, allowing your project to keep going. You can also make an explicit request for maintainers.

