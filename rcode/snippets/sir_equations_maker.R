sir_equations_maker <- function(functionName, contactMatrix, ...) {
  # Make all combinations of groups
  groupGrid <- expand.grid(...)
  groupGrid$group <- apply(groupGrid, 1, function(x) paste(x, collapse = "_"))
  groupList <- list()
  
  # Paste S, I and R to these groups
  compartments <- c("S", "I", "R")
  for (i in 1:length(groupGrid$group)) {
    groupList[[groupGrid$group[i]]] <- paste(compartments, groupGrid$group[i], sep = "_")
  }
  
  # Make the names of all essential parameters
  infectiousComps <- sapply(groupList, function(x) x[2])
  popComps <- gsub("^I", "N", infectiousComps)
  betaParams <- paste0("beta_", groupGrid$group)
  gammaParams <- paste0("gamma_", groupGrid$group)
  foiParams <- paste0("forceInfection_", groupGrid$group)
  
  # Generate strings that are going to be parse as equation
  funBody <- c()
  funBodyReturn <- c()
  for (i in 1:length(infectiousComps)) {
    funBody <- append(funBody, paste0(foiParams[i], " <- ", betaParams[i], " * (", 
                                      paste0(contactMatrix[i,], " * ", infectiousComps, 
                                             " / ", popComps, collapse = " + "),
                                      ")"))
    funBody <- append(funBody, paste0("d", groupList[[i]][1], " <- -", foiParams[i], " * ", groupList[[i]][1]))
    funBody <- append(funBody, paste0("d", groupList[[i]][2], " <- ", foiParams[i], " * ", groupList[[i]][1], " - ", gammaParams[i], " * ", groupList[[i]][2]))
    funBody <- append(funBody, paste0("d", groupList[[i]][3], " <- ", gammaParams[i], " * ", groupList[[i]][2]))
    funBodyReturn <- append(funBodyReturn, paste0("d", groupList[[i]][1]))
    funBodyReturn <- append(funBodyReturn, paste0("d", groupList[[i]][2]))
    funBodyReturn <- append(funBodyReturn, paste0("d", groupList[[i]][3]))
  }
  
  funBody <- append(funBody, paste0("return(list(c(", paste(funBodyReturn, collapse = ", "), ")))"))
  
  # Generate sir equation function
  eval(parse(text = paste0(rlang::enexpr(functionName), " <- function(time, variables, parameters) {",
                           "with(as.list(c(variables, parameters)), {",
                           paste(funBody, collapse = "; "),
                           "})",
                           "}")), 
       envir = .GlobalEnv)
}
