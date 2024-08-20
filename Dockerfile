FROM python:3.9-slim 
COPY wasm/dist/ service/
WORKDIR service
CMD [ "python3", "-m", "http.server", "80"] 
