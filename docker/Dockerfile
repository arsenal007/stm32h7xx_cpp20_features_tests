FROM alpine:3.22

# Copy setup script into container
COPY setup.sh /tmp/setup.sh
RUN chmod +x /tmp/setup.sh && /tmp/setup.sh && rm /tmp/setup.sh

WORKDIR /work
CMD ["bash"]