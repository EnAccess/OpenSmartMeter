# Define variables for KiBot
IMAGE_NAME := ghcr.io/inti-cmnb/kicad7_auto_full:latest
# specify platform to be sure we can run on Apple Silicons
DOCKER_RUN := docker run --rm --platform linux/amd64
DOCKER_VOLUME := -v "$(shell pwd)/hardware/pcb_cad_design":/mnt
DOCKER_WORKDIR := -w /mnt

# Define targets
.PHONY: kibot-all kibot-development kibot-production

kibot-all:
	$(DOCKER_RUN) $(DOCKER_VOLUME) $(DOCKER_WORKDIR) $(IMAGE_NAME) kibot all

kibot-development:
	$(DOCKER_RUN) $(DOCKER_VOLUME) $(DOCKER_WORKDIR) $(IMAGE_NAME) kibot development

kibot-production:
	$(DOCKER_RUN) $(DOCKER_VOLUME) $(DOCKER_WORKDIR) $(IMAGE_NAME) kibot production
