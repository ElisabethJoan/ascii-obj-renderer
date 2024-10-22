import { Button } from "@mui/material";
import { styled } from '@mui/material/styles';

const VisuallyHiddenInput = styled('input')({
  clip: 'rect(0 0 0 0)',
  clipPath: 'inset(50%)',
  height: 1,
  overflow: 'hidden',
  position: 'absolute',
  bottom: 0,
  left: 0,
  whiteSpace: 'nowrap',
  width: 1,
});

type FileUploadProps = {
    uploadFile: (file: FileList | null) => void;
}

export const FileUpload: React.FC<FileUploadProps> = ({ uploadFile }) => {
  return (
    <Button
      component="label"
      role={undefined}
      variant="outlined"
      tabIndex={-1}
    >
      Upload OBJ
      <VisuallyHiddenInput
        type="file"
        onChange={(event) => uploadFile(event.target.files)}
        multiple
      />
    </Button>
  );
}
