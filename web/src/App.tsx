import * as React from "react";
import { Button, Stack } from "@mui/material";

import { HomeRow } from "@elisabethjoan/portfolio-scaffold";

import { WASMLoader } from "./WASMLoader";
import { FileUpload } from "./FileUpload";

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
      <HomeRow extension={".tsx"} />
      <WASMLoader moduleName={"main"} args={[file]} />
      <Stack 
        spacing={2}
        padding={2}
        direction="row"
        alignContent="flex-end"
        justifyContent="center"
      >
        <Button variant="outlined" onClick={() => selectFile("assets/cube.obj")}>Cube</Button>
        <Button variant="outlined" onClick={() => selectFile("assets/torus10.obj")}>Torus</Button>
        <FileUpload uploadFile={uploadFile} />
      </Stack>
    </>
  )
}

export default App
