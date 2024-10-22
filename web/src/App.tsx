import * as React from "react";
import { Button, Stack } from "@mui/material";

import { WASMLoader } from "./WebAssemblyLoader";
import { FileUpload } from "./FileUpload";
import "./App.css"

function App() {
  const [file, setFile] = React.useState("");

  function selectFile(file: string) {
    setFile(file);
  }

  function uploadFile(file: FileList | null) {
    console.log(file);
  }


  return (
    <>
      <WASMLoader moduleName={"main"} objFile={file} />
      <Stack spacing={1} direction="row">
        <Button variant="outlined" onClick={() => selectFile("cube.obj")}>Cube</Button>
        <Button variant="outlined" onClick={() => selectFile("torus10.obj")}>Torus</Button>
        <FileUpload uploadFile={uploadFile} />
      </Stack>
    </>
  )
}

export default App
