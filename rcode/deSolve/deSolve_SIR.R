library(deSolve)
library(ggplot2)
library(tidyr)

discrete_p <- "/home/thinh/Dropbox/oucru/cpp/cpp_training/output/test"
outp <- "/home/thinh/Dropbox/oucru/cpp/plots"

days <- 20

# Time step
timeStep <- 0.001
timeStart <- 1 / timeStep + 2
timeEnd <- days / timeStep + 2

# Make the SIR model
sir_equations <- function(time, variables, parameters) {
  with(as.list(c(variables, parameters)), {
    dS <- -beta * I * S
    dI <-  beta * I * S - gamma * I
    dR <-  gamma * I
    return(list(c(dS, dI, dR)))
  })
}

# Input parameters
parameters_values <- c(
  beta  = 0.0015, # infectious contact rate (/person/day)
  gamma = 0.5    # recovery rate (/day)
)

# Input initial values
initial_values <- c(
  S = 999,  # number of susceptibles at time = 0
  I =   1,  # number of infectious at time = 0
  R =   0   # number of recovered (and immune) at time = 0
)

# Input time values
time_values <- seq(0, days) # days

# Solve the differential equations
sir_values_1 <- ode(
  y = initial_values,
  times = time_values,
  func = sir_equations,
  parms = parameters_values
)

# Make data frame from deSolve results
sir_values_1 <- data.frame(sir_values_1)
colnames(sir_values_1)[-1] <- paste0(colnames(sir_values_1)[-1], "_deSolve")

# Load our discrete time results
discrete_path <- list.files(discrete_p, pattern = ".csv", full.names = T)
l <- lapply(discrete_path, function(x) read.csv(x, header = T, stringsAsFactors = F, check.names = F))
df <- Reduce(function(x, y) merge(x, y, by = "Time", all = TRUE), l)

# Only get the time step we want
times <- seq(timeStart, timeEnd, 1 / timeStep)
df <- df[c(1, times),]
colnames(df)[-1] <- paste0(colnames(df)[-1], "_discrete")
df$Time[-1] <- (df$Time[-1] - 1) * timeStep

# Merge discrete data frame with deSolve data frame and visualize
df2 <- merge(sir_values_1, df, by.x = "time", by.y = "Time")
df_plot <- gather(df2, key = "Compartment", value = "Value", grep("time", colnames(df2), invert = T))
df_plot$package <- sapply(df_plot$Compartment, function(x) {a <- strsplit(x, "_")[[1]]; b <- a[length(a)]; return(b)})
df_plot$Compartment <- gsub("_deSolve|_discrete", "", df_plot$Compartment)
df_plot$Compartment <- factor(df_plot$Compartment)
ggplot(df_plot, aes(x = time, y = Value, col = Compartment)) +
  geom_line(size = 1) + facet_wrap(~ package) +
  theme_minimal() +
  theme(text = element_text(size = 14))

ggsave(file.path(outp, "basicSIR_discreteODE.pdf"), width = 7, height = 4)

df2

