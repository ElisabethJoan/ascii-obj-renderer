import * as React from "react";
import { Button, Stack } from "@mui/material";

import { WASMLoader } from "./WebAssemblyLoader";
import { FileUpload } from "./FileUpload";
import "./App.css"

function App() {
  const [file, setFile] = React.useState("assets/cube.obj");

  function selectFile(newFile: string) {
    setFile(newFile);
  }

  function uploadFile(newFile: FileList | null) {
    console.log(newFile);
  }


  return (
    <>
      <WASMLoader moduleName={"main"} args={[file]} />
      <Stack spacing={1} direction="row">
        <Button variant="outlined" onClick={() => selectFile("assets/cube.obj")}>Cube</Button>
        <Button variant="outlined" onClick={() => selectFile("assets/torus10.obj")}>Torus</Button>
        <FileUpload uploadFile={uploadFile} />
      </Stack>
    </>
  )
}

export default App
