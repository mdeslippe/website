# Welcome to My Website

This project contains the source code for my personal website, powered by a custom static site generator written in C.

The goal is to create a lightweight, durable website for showcasing my projects and interests. Most of the site's content is written in Markdown and automatically converted to HTML during the build process, making it easy to maintain and extend over time.

Rather than relying on complex frameworks or an ever-changing toolchain, the build system is designed with long-term stability in mind. By investing in the tooling upfront, the site can continue to be built for years to come with little to no maintenance.

## Project Layout

### Source Code

The `src` directory contains the implementation of the static site generator.

### Vendor Dependencies

The `vendor` directory contains all third-party dependencies, vendored into the repository to ensure reproducible builds and reduce reliance on external services.

## IDE Setup

Add the `vendor` directory to your IDE's include paths so IntelliSense can resolve vendored dependencies.
